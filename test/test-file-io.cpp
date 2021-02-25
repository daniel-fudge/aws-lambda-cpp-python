//
// Created by Daniel on 2021-02-24.
//

#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "yaml-cpp/yaml.h"

using namespace std;

TEST(FileIO, YAML){ // NOLINT(cert-err58-cpp)
    const char *name = "test.yml";

    ofstream yaml_file(name);

    string greeting = "Hey good looking!!!";
    yaml_file << "greeting: " << greeting << endl;
    yaml_file.close();

    YAML::Node input = YAML::LoadFile(name);

    ASSERT_EQ(input["greeting"].as<string>(), greeting);

    remove(name);
}
