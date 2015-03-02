# README #

### What is this repository for? ###

* This is the repo for the porenetwork generator build by Lucas van Oosterhout and Chaozhong Qin at the University of Utrecht.
* Version 1.0

### How do I get set up? ###

* The generator is setup by running make in the networkgen directory
* Add the moment there are no configuration steps needed to build
* A valid C++11 compiler *is* needed. The software builds with gcc-4.6 and clang-3.5 and higher, use other compilers at your own risk.
* At the moment the clang compiler is used, if you want to change this to gcc change the `CXX := clang++ `to `CXX:=g++` and you are set to go.

### Input ###
* For most cases executing the Generator file is enought, just make sure there is a `data` with an `NetworkSpecs.in` file in it. 

###The NetworkSpecs.in File ###
The `NetworkSpecs.in` file is pretty self-explantory. Comments lines can be added with the `#` character.


### Output ###
* A vtk file is produced for every flow direction.
* a connectivity file is generated
* a location file is generated
* a `_specs` file is generated, which contains information about the amount of porebodies, throats and number of `inlet` and `outlet` pores
* `inlet` porebodies are the first pores in the location list, and their connections are also at the head of the connectivity file.
* `outlet` porebodies are the last lines in the location list, and their connections are also at the tail of the connectivity file.



### Who do I talk to? ###
* For more information see the contact information at the github page of this project <https://github.com/luxinator/Pore-Network-Generator>
