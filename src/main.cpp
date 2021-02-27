//
// Created by Daniel on 2021-02-24.
//

#include "main.h"

int main(int argc, char **argv){
    
    if (argc != 4) {
        std::cout << "Require 3 arguments; bucket, object, region." << std::endl;
        return 1;
    }
    const std::string bucket_name = argv[1];
    const std::string object_name = argv[2];
    const std::string region_name = argv[3];

    YAML::Node input;
    
    if (!S3::ReadYaml(object_name, bucket_name, region_name, input)){
        std::cout << "Yaml file parsing failed." << std::endl;
        return 2;    
    } 
    
    std::string greeting = input["greeting"].as<std::string>();
    std::cout << greeting << std::endl;
    return 0;
}
