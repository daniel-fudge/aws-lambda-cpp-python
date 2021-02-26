# aws-lambda-cpp-python
This repo builds an AWS Lambda function with a Python handler that calls a C++ executable. 

## Environment Selection
In [this](https://youtu.be/olO5ORrq1cU) video we described the Cloud9 environment. The advantage of this environment is 
the environment is provided as a web-based service. If you wish to setup your own environment, you will need to install
the AWS CLI described [here](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-linux.html) and you should 
review the local C++ setup [here](https://github.com/daniel-fudge/aws-lambda-cpp-local-build).   
The remainder of these steps assume you have chosen the Cloud9 environment.

## Install Some Dependencies
Note the AWS Linux 2 AMI uses `yum` instead of `apt-get`. The version of CMake is also very old so we need to manually download a new version and install.
```bash
cd ~
sudo yum update -y
sudo yum install libcurl-devel -y
wget https://cmake.org/files/v3.19/cmake-3.19.5.tar.gz
tar -xvzf cmake-3.19.5.tar.gz 
cd cmake-3.19.5
./bootstrap
sudo make
sudo make install
```

## Build the AWS C++ SDK
These steps install the C++ SDK into the `~/install` directory and clones all the directories into `~/environment`. This can obviously be altered if desired.    
We are also only installing the `s3` package. Other packages may be required for more involved Lambda functions. 

Because we are using the `s3` package, which requires more memory to build the `t2.mirco` will not be sufficient. I used a `t2.medium`. 
You can change the instance type back to a `t2.micro` after the SDK is built.

```bash
mkdir ~/install
cd ~/environment
git clone https://github.com/aws/aws-sdk-cpp.git
cd aws-sdk-cpp
mkdir build
cd build
cmake .. -DBUILD_ONLY="s3" \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=OFF \
  -DENABLE_UNITY_BUILD=ON \
  -DCUSTOM_MEMORY_MANAGEMENT=OFF \
  -DCMAKE_INSTALL_PREFIX=~/install \
  -CPP_STANDARD=14
make
make install
```

## YAML-Cpp Setup
This simple C++ application reads a YAML file with a 3rd party package. To setup the package use the following steps.
1. Clone the YAML-CPP repo from [here](https://github.com/jbeder/yaml-cpp.git).
2. Place repo in `third-party` sub-folder
Make sure following lines are in [CMakeLists.txt](CMakeLists.txt)

```bash
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
include_directories(third-party/yaml-cpp/include)
add_subdirectory(third-party/yaml-cpp/ yaml-cpp)
```

## Local vs Cloud9 development
The Cloud9 enviroment is great for testing the AWS integration but it isn't as rich as a local IDE such as CLION with full GoogleTest integration.
Therefore in the top [CMakeLists](CMakeLists.txt) file toggle the `set(CLOUD9 ON)` line to `ON` or `OFF`. This control if the SDK is included or
GoogleTest along with any other dependencies.

## Build the Actual C++ Lambda Function
```bash
git clone git@github.com:daniel-fudge/aws-lambda-cpp-python.git
cd ~/aws-lambda-cpp-python
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/install
make
make aws-lambda-package-demo
```

## References
- [Local C++ Lambda Repo](https://github.com/daniel-fudge/aws-lambda-cpp-local-build)
- [Cloud9 C++ Lambda Repo](https://github.com/daniel-fudge/aws-lambda-cpp-cloud9)
- [AWS CLI - Installation](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-linux.html)
- [AWS CLI - Invoke Lambda](https://docs.aws.amazon.com/cli/latest/reference/lambda/invoke.html#examples)
- [AWS CLI - Payload Error](https://stackoverflow.com/questions/60310607/amazon-aws-cli-not-allowing-valid-json-in-payload-parameter)
- [AWS Lambda Runtimes](https://docs.aws.amazon.com/lambda/latest/dg/lambda-runtimes.html)
- [YAML-CPP Repo](https://github.com/jbeder/yaml-cpp)
- [Google Test]()
  
- [Introducing the C Lambda Runtime](https://aws.amazon.com/blogs/compute/introducing-the-c-lambda-runtime/)
- [C++ Sample Lab](https://github.com/awslabs/aws-lambda-cpp)
- [AWS EC2 Pricing](https://aws.amazon.com/ec2/pricing/on-demand/)
