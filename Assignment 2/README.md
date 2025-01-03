# Development Process of Software: Assignment 2
#### Group members
| Name | Email | Id |
| ------ | ------ | ------ |
| Mattia Bolognini | m.bolognini6@campus.unimib.it | 870401 |
| Giacomo Maggioni | g.maggioni24@campus.unimib.it | 869265 |
#### Gitlab Repository's URL
The repository is called "2024_assignment2_BM" and its URL is https://gitlab.com/uni6520048/2024_assignment2_bm.

NOTE: commits performed on this repo are all authored by Mattia Bolognini, but the group worked together and committed using a single account. All members are proficient in Git and all members learned how to implement a pipeline in Gitlab.\
NOTE: previous to the partial assignment's virtual class, commit messages were written without following any convention, like Conventional Commit. After prof. Tundo's comment on commit messages in the partial assignment's virtual class, we started to write commit messages with a more defined structure.

## Introduction
The assignment asks to define a Gitlab CI/CD pipeline to automate the development process of a chosen application. 
The pipeline has to be structured in the following 7 stages:
- build: resolve dependencies and compile the code
- verify: run static and dynamic analysis
- test: run unit/integration/performance tests
- package: generate a package ready to be released
- release: make the package available
- docs: generate and publish updated documentation
- deploy (optional): deploy the software to the field

The application chosen for the assignment is a simplified version of a Qt C++ application (originally hosted on [this] Github repo) which goal is to manage the collection of paintings of the Uffizi Galleries in Florence. The offered functionalities are the followings:
- visualize the paintings of the collection
- add a new painting
- remove a painting
- search for paintings that satisfy some filter-based criteria on the title


## CI/CD Pipeline
As already introduced in the previous section, the required CI/CD pipeline has to be structured in 7 stages that cover all important features a standard pipeline should have. Because of using C++ as the language of the application, some difficulties were met in its implementation. The most impactful one is that C++ has no concept of a package and the standard does not define a package manager. Dependencies management in C++ is also notoriously complex.
Package managers for C++ exist, but are rarely used and are less intuitive than package managers for other languages, like Cargo for Rust or pip for Python. External libraries are usually copy-and-pasted in the source tree, compiled from source and linked to using the project built tool or downloaded in binary form (this last approach requires to download different versions of the library binary for each of the target platform we want to compile the project for, because of different ABIs). For a more in-depth explanation of C++ dependecies management, see [C++ Deps Management].

To keep the pipeline simple, this project does not use a package manager. The only external libraries used, [Google Test] and [Google Benchmark], are automatically downloaded from their Github repo and compiled from source, generating static libraries in .a format. Artifacts in the pipeline, such as the makefile, then refer to their headers and the static libraries previously compiled.

The tools used in the pipeline are:
- [make] - build tool of the project
- [googletest] - test library for C++
- [benchmark] - microbenchmark library for C++
- [cpplint] - Python package for linting of C++ source files, following Google coding convention
- [valgrind] - memory checker for C++ and Java
- [docker] - containerization software
- [doxygen] - tool to generate C++ documentation from comments in the source files

The default Docker image used by the pipeline is the gcc image, containing basic tools for C++ code building, like gcc and make.

NOTE: all intermediate build results are saved as artifacts, as suggested by [Gitlab's guidelines]. Caches, usually used to save dependencies, are not necessary in this pipeline because dependencies are used in one job only and compiled-from-source, as you'll see.

#### build
The goal of the build stage is to compile and link the source files, creating an executable file using the gcc docker image as the compilation target.
The build stage uses the make tool and a dedicated file, called Makefile, containing rules that instruct make on how to perform the compilation and the eventual linking. 

This stage is composed of two jobs: building of the final application and building of the test application. Tests are not included in the deployable application because they are useful only for debugging purposes, and their inclusion in the final executable would only increment the memory required for its execution. Unit tests and benchmarks are built into a single executable to lessen the burden on the required memory, but in future jobs will be conditionally executed using some filters.

Unit tests are implemented using googletest external library, while benchmarks are implemented using benchmark external library, also from Google. Therefore, the second job has to download their source code and compile them from source before executing the correct makefile rule. To do this, two different scripts are used.

Both jobs save their artifacts (final application and test executable) for later use in following jobs of the pipeline.

#### verify
In the verify stage, both static analysis of the source code and dynamic analysis are performed, using two different jobs. Static analysis consists of code linting, while dynamic analysis performs memory checking.

For code linting, cpplint Python package is used. Because cpplint is a Python package, the static analysis job requires Python and pip for its installation: consequently, this job uses a different docker image already containing both of them, and then it installs the required package using pip. Linting is then performed following Google coding convention.
Because linting is an "optional" aspect of the code correctness (code not following linting convention could be correct and could implement all of the required functional requirements), the job is allowed to fail.

The memory checking job uses valgring instead, which is obviously installed before performing the analysis.
Memory checking is usually performed while the application is in use, receiving inputs and changing its state. To simulate interactions with the program, unit tests are used: therefore, the dynamic analysis is performed not on the final application, but on the test application.
The implemented tests are simple and do not test all the code of the project, and the memory checking is therefore incomplete. In a real-world scenario, the application used for dynamic analysis should perform interactions with all parts of the code to try to find the most bugs possible.
This job uses an artifact, the test application, produced by the compile-test job of the build stage: dependencies are not an empty set.

#### test
The test stage performs both unit tests and performance benchmarks on the code by executing the test executable produced in the build stage.
This stage does not require additional notes: the two jobs simply executes the test application, one on the unit tests and the other on the benchmarks.
Benchmarks are resource-intensive and, for this reason, they are executed only when new code is published to the main branch and only if all unit tests succeed.
Because these jobs uses an artifact from the compile-test job of the build stage, they depend on it.

#### package
The package stage should create a package of the application, later to be published. Because C++ has no standard concept of package, and the pipeline does not use any package manager, the job of the release stage is limited only to the zipping of the final application's executable, which is performed only when new code is published to the main branch.

#### release
The release stage performs the creation of the docker image, to be published on the repo's Container Registry, and the defition of a new release referring to the previously zipped executable.
The stage is composed of three different jobs: persist, docker-image and release.
All three jobs can be executed in parallel, optimizing execution times of the pipeline.

The persist job saves the previously zipped final application's executable in the generic package repo associated to the Gitlab repo. To do so, the job sends an api request to Gitlab, containing both the api token to access the generic package repo and the file to be saved.
This job saves the artifact produced by the zip job of the previous stage, and therefore it depends on it.
The api token is a sensitive information that must be protected at all costs to not compromise the repo security. The most secure approach, but also most difficult to implement, uses external secrets, third-party vaults that store sensitive info and return them only when some type of authentication is performed. A easier-to-implement approach is to save the data as a hidden and masked variable of the repo, meaning its value cannot be seen anymore after its creation and it does not appear in any pipeline's log. We chose to save the data as a hidden and masked variable, protecting the api token without too much harsh in the implementation.

The docker-image job uses a Docker image containing both Docker and its Docker-in-Docker (dind) service to create a Docker image containing only the final application's executable (the application has no dependencies to dynamic libraries, and no external library has to be installed in the image), following the instructions contained in the Dockerfile. It then publish the new Docker image to the Container Registry associated to the repo.
The job creates the image by adding a layer containing an artifact of the compile-prog job of the build stage and, therefore, depends on the compile-prog job.

The release job defines a new release and implicitly tags the repo. 
To publish a new release, the job has a release section, containing some info and a link to the location of the generic package repo in which the zipped final application was saved.
To name the release, multiple options are available. The most traditional one is  semantic versioning, in which releases are named using incremental variables representing their versions. However, this approach is really hard to implement for C++ in comparison to other languages like Python. Therefore, releases created with this pipeline are named after their commit short SHA, to uniquely identify them. 

The release stage is performed only when new code is pushed/merged to main: releases should not be performed at every single commit.

#### docs
The docs stage executes only one job, the pages job: it generates the documentation in the html/ folder using doxygen and then moves its content to the public folder. The public folder is used by Gitlab to identify all .html files that have to be published to the Gitlab pages website associated to the current repo. Before permorfing the documentation generation, the job has to install doxygen.
This job is only executed when new code is pushed/merged to the main branch: documentation should, in fact, change only at new releases, not at every commit performed by a single developer. Because the pages stage follows the release stage, new documentation is published only if the release of the new application is successful.
Reminder: the job that publish some files to the Gitlab pages website has to be called pages. Other names do not allow automatic publication from Gitlab.

The repo website can be found at this [URL].
If you are reading the PDF version of this README.md, the URL is https://2024-assignment2-bm-6984ea.gitlab.io/.

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)
   
   [this]: <https://github.com/mattia010/qt-uffizi>
   [.gitlab-ci.yml]: <https://gitlab.com/uni6520048/2024_assignment2_bm/-/blob/main/.gitlab-ci.yml?ref_type=heads>
   [Google Test]: <https://github.com/google/googletest>
   [Google Benchmark]: <https://github.com/google/benchmark>
   [googletest]: <https://github.com/google/googletest>
   [benchmark]: <https://github.com/google/benchmark>
   [C++ Deps Management]: <https://filebox.ece.vt.edu/~clwyatt/Essays/2023-03-03-cpp-link-deps/>
   [cpplint]: <https://github.com/cpplint/cpplint>
   [valgrind]: <https://valgrind.org/>
   [docker]: <https://www.docker.com/>
   [doxygen]: <https://www.doxygen.nl/>
   [URL]: <https://2024-assignment2-bm-6984ea.gitlab.io/>
   [Gitlab's guidelines]: <https://docs.gitlab.com/ee/ci/caching/>
