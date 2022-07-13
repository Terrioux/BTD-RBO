# BTD/RBO solvers

BTD and RBO are open-source solvers which handle respectively CSP and COP instances.


# Download and installation

As your convenience, you can download an executable jar (from our [release](https://github.com/Terrioux/BTD-RBO/releases) repository) or the [source code](https://github.com/Terrioux/BTD-RBO).

BTD/RBO solvers have the following requirements:
* C++ 11 or later

## Building instructions
First, download the sources or clone the repository, for example, by executing the following command:  

    git clone https://github.com/Terrioux/BTD-RBO.git
    
Then, from the source directory, execute the following command:  

    make
    
The generated executables are located:
* in the directory CP/exe for BTD and RBO	(intended for the CSP/COP standard track of the XCSP competition)
* in the directory miniCP/exe for miniBTD and miniRBO	(intended for the CSP/COP mini track of the XCSP competition)

## Running solvers
Solvers can be run by executing the following command:  

    SOLVER TIMELIMIT BENCHNAME

where:
* SOLVER is the path to the executable BTD or miniBTD,
* TIMELIMIT is the number of seconds allowed for solving the instance,
* BENCHNAME is the name of the XML file representing the instance we want to solve.

# Third-party

The BTD/RBO solvers rely on several third-party open-source libraries, each being licensed under its own license (see the [https://github.com/Terrioux/BTD-RBO/blob/main/3rd-party-licenses.md](https://github.com/Terrioux/BTD-RBO/blob/main/3rd-party-licenses.md) files for more details).
* [XCSP3 Core Parser in C++](https://github.com/xcsp3team/XCSP3-CPP-Parser)


# Directories
* CP: the source files of the constraint solvers developed for the CSP/COP standard track of the XCSP competition
* Graph: our graph library 
* XCSP3-Parser: a variant of the XCSP3 Core Parser in C++, derived from [https://github.com/xcsp3team/XCSP3-CPP-Parser](https://github.com/xcsp3team/XCSP3-CPP-Parser)
* miniCP: the source files of the constraint solvers developed for the CSP/COP mini track of the XCSP competition
