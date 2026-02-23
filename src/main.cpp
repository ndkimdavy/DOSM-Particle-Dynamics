#include "dosmMetric.hpp"
#include "dosmConfig.hpp"

#include <string>
#include <stdexcept>
#include <cctype>

int main(int argc, char** argv)
{
    using namespace dosm;

    auto print_help = [&]() {
        DOSM_LOG_INFO(
            "Usage:\n"
            "  --law LJ|LJP|LJPNB|LJPNBCL\n"
            "  --input <file>\n"
            "  --mci <file>        (optional, exam initial momenta)\n"
            "  --nomci             (force disable mci)\n"
            "  --csv <file> --pdb <file>\n"
            "  --seed <u32>\n"
            "  --mass <r64> --charge <r64>\n"
            "  --sigma <r64> --epsilon <r64>\n"
            "  --length <r64> --rcut <r64> --skin <r64>\n"
            "  --dt <r64> --steps <usize>\n"
            "  --gridx <usize> --gridy <usize>\n"
            "  --stepevery <usize>\n"
            "  --thermostat 0|1\n"
            "  --stepsocket <usize>\n"
            "  --ip <string> --port <u16>\n"
        );
    };

    try
    {
        if (argc == 1)
        {
            print_help();
            return 0;
        }

        for (int i = 1; i < argc; ++i)
        {
            std::string a = argv[i];
            auto need_value = [&](const char* opt) -> std::string {
                if (i + 1 >= argc)
                    throw std::runtime_error(std::string("Missing value for ") + opt);
                return std::string(argv[++i]);
            };

            if (a == "--help" || a == "-h")
            {
                print_help();
                return 0;
            }
            else if (a == "--law")        config.law = need_value("--law");
            else if (a == "--input")      config.inputFile = need_value("--input");
            else if (a == "--mci")        config.mciFile = need_value("--mci");
            else if (a == "--nomci")      config.mciFile.clear();
            else if (a == "--csv")        config.csvFile = need_value("--csv");
            else if (a == "--pdb")        config.pdbFile = need_value("--pdb");
            else if (a == "--seed")       config.seed = (ui32_t)std::stoul(need_value("--seed"));
            else if (a == "--mass")       config.mass = std::stod(need_value("--mass"));
            else if (a == "--charge")     config.charge = std::stod(need_value("--charge"));
            else if (a == "--sigma")      config.sigma = std::stod(need_value("--sigma"));
            else if (a == "--epsilon")    config.epsilon = std::stod(need_value("--epsilon"));
            else if (a == "--length")     config.boxLength = std::stod(need_value("--length"));
            else if (a == "--rcut")       config.rayCut = std::stod(need_value("--rcut"));
            else if (a == "--skin")       config.skin = std::stod(need_value("--skin"));
            else if (a == "--dt")         config.dt = std::stod(need_value("--dt"));
            else if (a == "--steps")      config.steps = (idx_t)std::stoull(need_value("--steps"));
            else if (a == "--gridx")      config.gridDimX = (idx_t)std::stoull(need_value("--gridx"));
            else if (a == "--gridy")      config.gridDimY = (idx_t)std::stoull(need_value("--gridy"));
            else if (a == "--stepevery")  config.stepEvery = (idx_t)std::stoull(need_value("--stepevery"));
            else if (a == "--thermostat") config.thermostat = (bool)std::stoul(need_value("--thermostat"));
            else if (a == "--stepsocket") config.stepSocket = (idx_t)std::stoull(need_value("--stepsocket"));
            else if (a == "--ip")         config.ip = need_value("--ip");
            else if (a == "--port")
            {
                auto value = std::stoul(need_value("--port"));
                if (value > 65535)
                    throw std::runtime_error("Port out of range");
                config.port = (ui16_t)value;
            }
            else
                throw std::runtime_error("Unknown option: " + a);
        }

        for (char& c : config.law)
            c = (char)std::toupper((unsigned char)c);

        DosmMetric dosmMetric;
        dosmMetric.run();
        return 0;
    }
    catch (const std::exception& e)
    {
        DOSM_LOG_ERROR(e.what());
        print_help();
        return 1;
    }
}