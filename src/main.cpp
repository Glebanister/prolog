#include <fstream>

#include "cxxopts.hpp"

#include "PrologGrammarChecker.hpp"

int runTests()
{
    std::vector<std::string> correctProgramms = {
        "",
        "a:-b.",
        "a.",
        "a :- x.",
        "a :- (x).",
        "a :- x, y, z.",
        "a :- x, y; z.",
        "a :- x, (y; z).",
        "other_name :- x, ((y; z)).",
        "other_name :- x, (y; (a, b, c, d, e; f) ,z).",
        "x :- x, (y; (a, b, c, d, (e); f) ,z).",
        "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d.",
        "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; d.",
        "x :- x, (y; (a, b, c d, (e); f) ,z).",
        "a :- a a a a a.",
        "a :- a a a a a, a a a; a a a, a.",
        "a (((a))) :- (((a))), a.",
        "a (a) a ((a)) :- a, a, a (a).",
        "a :- (a).",
        "a :- (a), (b), (c)."};
    std::vector<std::string> incorrectProgramms = {
        "1",
        ".",
        ":-",
        "a:-",
        "a:-1.",
        "a:-.",
        ":-.",
        "a :- (x, y, z.",
        "a :- x, y))) z.",
        "(a :- x, ,y; z.)",
        "(a :- x, ,y; z.).",
        "a :- 1.",
        "a :- x..",
        "1badname :- x, (y; z).",
        "other_name :- x, (y; ((a, b, c, d, e; f) ,z).",
        "x :- x, (y; a, b, c, d, (e); f) ,z), a ; d.",
        "x :- x, (y; (a, b, c, d, (e); f) ,z), a ; .",
        "a :- (a) (a) a a a.",
        "a (((a)) :- (((a))), a.",
        "a (a) a ((a)) :- a, a, a ((a) a (a)).",
        "a :- ().",
        "(a) :- a.",
        "a () :- (a), (b), (c)."};

    int result = 0;
    for (const auto correctTest : correctProgramms)
    {
        std::vector<std::shared_ptr<prolog::exception::Exception>> exceptions;
        for (auto maybeException : prolog::grammar::checkPrologProgram(correctTest))
        {
            if (auto ex = std::dynamic_pointer_cast<prolog::exception::Exception>(maybeException); ex)
            {
                exceptions.push_back(std::move(ex));
            }
        }
        if (!exceptions.empty())
        {
            std::cout << "Program:" << std::endl
                      << "'" << correctTest << "' is correct, but has exceptions" << std::endl;
            for (const auto &ex : exceptions)
            {
                std::cout << ex->what() << std::endl;
            }
            result = -1;
        }
    }

    for (const auto incorrectTest : incorrectProgramms)
    {
        auto exs = prolog::grammar::checkPrologProgram(incorrectTest);
        if (exs.empty())
        {
            std::cout << "Program:" << std::endl
                      << "'" << incorrectTest << "' is not correct, but has not exceptions" << std::endl;
            result = -1;
        }
    }
    return result;
}

int main(int argc, const char **argv)
{
    try
    {
        std::string programText;
        std::string outputFilename;
        {
            const std::string description = ""
                                            "Prolog grammar checker\n"
                                            "Specify input file path, checker will put error information to stdout"
                                            "\nor empty string if program is correct Prolog program\n";

            cxxopts::Options options("prolog-checker", description);

            std::string inputFilename;

            options.add_options()(
                "i,input", "Specify input file path", cxxopts::value<std::string>(inputFilename))(
                "o,output", "Specify output file path", cxxopts::value<std::string>(outputFilename))(
                "h,help", "Print usage")(
                "t,test", "Run tests");

            auto result = options.parse(argc, argv);
            if (result.count("help"))
            {
                std::cout << options.help() << std::endl;
                return 0;
            }

            if (result.count("test"))
            {
                return runTests();
            }

            if (!result.count("input"))
            {
                throw std::invalid_argument("Option input is required, add -h to see help");
            }

            std::ifstream inputFile(inputFilename);
            if (!inputFile.good())
            {
                throw std::invalid_argument("Can not read from '" + inputFilename + "'");
            }
            programText = std::string((std::istreambuf_iterator<char>(inputFile)),
                                      std::istreambuf_iterator<char>());
            if (outputFilename.empty())
            {
                outputFilename = inputFilename + ".out";
            }
        }

        std::vector<std::string> lines;
        std::stringstream codestream(programText);
        std::string line;
        while (std::getline(codestream, line, '\n'))
        {
            lines.push_back(std::move(line));
        }
        std::stringstream output;
        std::size_t lineId = 0;
        for (auto lineResult : prolog::grammar::checkPrologProgram(programText))
        {
            output << lines[lineId] << std::endl;
            if (auto treePtr = std::dynamic_pointer_cast<prolog::SyntaxTree>(lineResult); treePtr)
            {
                treePtr->print(output);
            }
            else if (auto posex = std::dynamic_pointer_cast<prolog::exception::PositionalException>(lineResult); posex && posex->line < lines.size())
            {
                for (std::size_t i = 0; i < posex->linePos; ++i)
                {
                    output << "-";
                }
                output << "^";
                for (std::size_t i = posex->linePos + 1; i < lines[posex->line].length(); ++i)
                {
                    output << "-";
                }
                output << std::endl;
                posex->print(output);
            }
            else if (auto ex = std::dynamic_pointer_cast<prolog::exception::Exception>(lineResult); ex)
            {
                ex->print(output);
            }
            else
            {
                throw std::invalid_argument("Unknown parsing result returned from prolog checker");
            }
            if (++lineId != lines.size())
            {
                output << std::endl;
            }
        }
        std::ofstream file(outputFilename);
        if (!file.good())
        {
            throw std::invalid_argument("Can not write to '" + outputFilename + "'");
        }
        file << output.str();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
