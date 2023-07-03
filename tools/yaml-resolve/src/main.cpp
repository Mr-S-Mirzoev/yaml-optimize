#include "cli.h"
#include "resolver.h"

#include <fstream>
#include <iostream>

#include "fmt/format.h"

#ifdef YO_DEBUG
#pragma message("Building debug version of resolver")
#else
#pragma message("Building release version of resolver")
#endif

void run(int argc, char* argv[])
{
    cli_utils::Args args{};
    if (!cli_utils::parse(argc, argv, args))
        exit(0);

    std::ifstream is(args.in_fname);

    YamlResolver resolver(is);

    resolver.resolve();

    resolver.dump(args.out_fname);
}

int main(int argc, char* argv[])
{
    try
    {
        run(argc, argv);
    }
    catch (const cli_utils::ArgParseError& e)
    {
        std::cerr << fmt::format("Error during CLI args parsing occured: {}",
                                 e.what())
                  << std::endl;
        return 1;
    }
    catch (const YamlResolverError& e)
    {
        std::cerr << fmt::format("Error during optimization occured: {}",
                                 e.what())
                  << std::endl;
        return 1;
    }

    return 0;
}
