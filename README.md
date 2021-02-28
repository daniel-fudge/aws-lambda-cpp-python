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
  -DCMAKE_INSTALL_PREFIX=~/install
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
cd ~/environment/aws-lambda-cpp-python
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/install
make
make aws-lambda-package-demo
```

## Make IAM Role for the Lambda Function
We need to create and IAM role that the can be attached to the lambda function when it is deployed. 
Note the rights in this role may need to be expanded for more complex Lambda functions.  
First create a JSON file that defines the required permissions as shown below.
```JSON
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": {"Service": ["lambda.amazonaws.com"]},
      "Action": "sts:AssumeRole"
    }
  ]
}
```
Then create the IAM role in the CLI as shown below.
```bash
aws iam create-role --role-name lambda-demo --assume-role-policy-document file://trust-policy.json
```
The output of the above command will include the ARN of the new role. You must copy this ARN. It will be required when you deploy the Lambda function. 
It will most like have the form `arn:aws:iam::<your AWS account number>:role/lambda-demo`.   

Next attached the `AWSLambdaBasicExecutionRole` and `AmazonS3FullAccess` policies to the new role to allow the Lambda function to write to CloudWatch Logs and access the S3 bucket. 
This is performed with the following CLI commands.
```bash 
aws iam attach-role-policy --role-name lambda-demo --policy-arn arn:aws:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole
aws iam attach-role-policy --role-name lambda-demo --policy-arn arn:aws:iam::aws:policy/service-role/AmazonS3FullAccess
```

## Zip and Deploy the Lambda Function 
We can now deploy the lambda function with the following CLI command.
```bash
cd ~/environment/aws-lambda-cpp-python
mkdir package
cd package
cp ../lambda_function.py .
cp ../build/src/cpp-read-yaml_run cpp.exe
cp /usr/lib64/libcurl.so.4 .
cp /usr/lib64/libnghttp2.so.14 .
cp /usr/lib64/libidn2.so.0 .
cp /usr/lib64/libssh2.so.1 .
cp /usr/lib64/libldap-2.4.so.2 .
cp /usr/lib64/liblber-2.4.so.2 .
cp /usr/lib64/libunistring.so.0 .
cp /usr/lib64/libsasl2.so.3 .
cp /usr/lib64/libssl3.so .
cp /usr/lib64/libsmime3.so .
cp /usr/lib64/libnss3.so .
zip package.zip *
aws lambda create-function \
  --function-name demo-cpp-python \
  --role arn:aws:iam::[your account id]:role/lambda-demo \
  --runtime python3.8 --timeout 10 --memory-size 80 \
  --handler lambda_function.lambda_handler \
  --zip-file fileb://package.zip
cd ..
rm -fr package
```

It may also be useful to update the package through the CLI with the followig command.
```bash
aws lambda update-function-code --function-name  demo-cpp-python --zip-file fileb://package.zip
```

## Create the S3 Trigger
From the above command or the Lambda console you will need the Lambda function ARN. This will replace the `lambda-arn` in the following json file.

First we add a resource policy to give s3 permission to invoke the lambda function. You will have to use a different S3 ARN and account ID. 
See the [docs](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/lambda/add-permission.html) for a full set of options.
```bash
aws lambda add-permission \
  --function-name demo-cpp-python \
  --statement-id s3-trigger \
  --action "lambda:InvokeFunction" \
  --principal s3.amazonaws.com \
  --source-arn "arn:aws:s3:::your-bucket" \
  --source-account "your-acc-id"
```

To add the trigger we actually add an event configuration to the S3 bucket, which triggers the Lambda function added above.  
First we create the `s3-trigger.json` config file as shown below (replace the ARN).
```json
{
  "LambdaFunctionConfigurations": [
    {
      "LambdaFunctionArn": "lambda-arn",
      "Events": ["s3:ObjectCreated:*"]
    }
  ]
}
```
This configuration is then added to the S3 buccket with the following command. Note that you will have to change your bucket name.
```bash
aws s3api put-bucket-notification-configuration --bucket lambda-cpp-test  --notification-configuration file://s3-trigger.json
```

## Test the S3 upload and Lambda trigger
```bash
aws s3 cp test.yml s3://lambda-cpp-test
```

## References
- [Local C++ Lambda Repo](https://github.com/daniel-fudge/aws-lambda-cpp-local-build)
- [Cloud9 C++ Lambda Repo](https://github.com/daniel-fudge/aws-lambda-cpp-cloud9)
- [AWS CLI - Installation](https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2-linux.html)
- [AWS CLI - Add permissions](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/lambda/add-permission.html)
- [AWS CLI - Invoke Lambda](https://docs.aws.amazon.com/cli/latest/reference/lambda/invoke.html#examples)
- [AWS CLI - Payload Error](https://stackoverflow.com/questions/60310607/amazon-aws-cli-not-allowing-valid-json-in-payload-parameter)
- [AWS S3 API](https://awscli.amazonaws.com/v2/documentation/api/latest/reference/s3api/put-bucket-notification-configuration.html)
- [AWS Zip Deployment](https://docs.aws.amazon.com/lambda/latest/dg/python-package.html)
- [AWS Lambda Runtimes](https://docs.aws.amazon.com/lambda/latest/dg/lambda-runtimes.html)
- [YAML-CPP Repo](https://github.com/jbeder/yaml-cpp)
- [Introducing the C Lambda Runtime](https://aws.amazon.com/blogs/compute/introducing-the-c-lambda-runtime/)
- [C++ Sample Lab](https://github.com/awslabs/aws-lambda-cpp)
- [AWS EC2 Pricing](https://aws.amazon.com/ec2/pricing/on-demand/)
