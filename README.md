# aws-lambda-cpp-python
This repo builds an AWS Lambda function with a Python handler that calls a C++ executable. 

## Environment Selection
In [this](https://youtu.be/olO5ORrq1cU) video we described the Cloud9 environment. The advantage of this environment is 
the environment is provided as a web-based service. If you wish to setup your own environment, you will need to install
the AWS CLI described [here](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-linux.html) and you should 
review the local C++ setup [here](https://github.com/daniel-fudge/aws-lambda-cpp-local-build).   
The remainder of these steps assume you have chosen the Cloud9 environment.

## YAML-Cpp Setup
This simple C++ application reads a YAML file with a 3rd party package. To setup the package use the following steps.
1. Clone the YAML-CPP repo from [here](https://github.com/jbeder/yaml-cpp.git).
2. Place repo in `third-party` sub-folder
Make sure following lines are in [CMakeLists.txt]()
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
