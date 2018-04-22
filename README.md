# Stride project

|CodeFactor|Travis|Clang 5|GCC 6| GCC 7|
|--|--|--|--|--|
|[![CodeFactor](https://www.codefactor.io/repository/github/ledfan/bachelorproef/badge)](https://www.codefactor.io/repository/github/ledfan/bachelorproef)|[![Build Status](https://travis-ci.com/LEDfan/Bachelorproef.svg?token=csyiPstpMdAYGLr6wqxd&branch=master)](https://travis-ci.com/LEDfan/Bachelorproef)|[![Build Status](https://ci.ledfan.be/buildStatus/icon?job=BachelorProef/Tests-compile-docker-clang-5)](https://ci.ledfan.be/job/BachelorProef/job/Tests-compile-docker-clang-5/)|[![Build Status](https://ci.ledfan.be/buildStatus/icon?job=BachelorProef/Tests-compile-docker-gcc-6)](https://ci.ledfan.be/job/BachelorProef/job/Tests-compile-docker-gcc-6/)|[![Build Status](https://ci.ledfan.be/buildStatus/icon?job=BachelorProef/Tests-compile-docker-gcc-7)](https://ci.ledfan.be/job/BachelorProef/job/Tests-compile-docker-gcc-7/)|

Stride is an Open Source project to simulate transmission of infectious diseases.

The project is hosted in a GitHub [code repository](https://github.com/broeckho/stride.git) and it has a continous integration cycle [build & test](https://travis-ci.org/broeckho/stride) at Travis CI.

Information on layout, installation, dependencies of the project, and platforms that are supported, can be found in the files INSTALL.txt, DEPENDENCIES.txt and PLATFORMS.txt respectively.
For license info, see LICENSE.txt.

Current contributors are (in alphabetical order):

* Niels Aerens ([@PurgingPanda](https://github.com/PurgingPanda))
* Thomas Avé  ([@thomasave](http://github.com/thomasave))
* Tobia De Koninck ([@LEDfan](https://github.com/LEDfan))
* Robin Jadoul ([@RobinJadoul](https://github.com/RobinJadoul))

Contact Person:

* Name: Tobia De Koninck
* Github: ([@LEDfan](https://github.com/LEDfan))
* E-Mail: tobia.dekoninck@student.uantwerpen.be

Upstream:
* Jan Broeckhove (jan.broeckhove@uantwerpen.be)
* Elise Kuylen (elise.kuylen@student.uantwerpen.be)
* Lander Willem (lander.willem@uantwerpen.be)

## GenGeo
The GenGeo can be tested by compiling Stride and then running the `./cmake-build-release/installed/bin/gengeopop` program.
This executable can be given some files as parameter (use `gengeopop --help to show the usage`), these paths are relative to the data directory.
By default the program will use the following files:
 - `flanders_cities.csv`
 - `flanders_commuting.csv`
 - `households_flanders.csv`
The GeoGrid will be written into `gengeopop.json`, which can be opened using the Visualiser.


## GenGeo visualiser
The visualiser is compiled with the other binaries. It can run by `./cmake-build-release/installed/bin/visualizer`. You then have to load a geogrid file by clicking the load file button on the right hand side. The map on the left hand side shows all the locations in the geogrid. You can get more info by clicking on a marker. The middle column will then have the info about the location and the contact centers. You can click on one of the contact centers in that list, you will then get info about it in the right column.
You can select multiple locations by using SHIFT.

The current status of the project build is : ![Build Status](https://travis-ci.com/LEDfan/Bachelorproef.svg?token=AhWiySeGEDkQfLDToshu&branch=master)

## Integration

The Stride binary can use a Population in three ways:
  
  - `default`   Generate a Population using `PopulationBuilder` (use `./bin/stride`)
  - `import`    Import a Population from a `proto` or `json` file, which was generated using `gengeopop`. (use `./bin/stride -c run_import_default.xml`)
  - `generate`  Generate a Population using GenGeoPop and provide data files and parameters. (use `./bin/stride -c run_generate_default.xml`)
