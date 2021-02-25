//
// Created by Daniel on 2021-02-24.
//

#include "main.h"

int main(int argc, char **argv){
    string input_name = "input.yml";

    YAML::Node input = YAML::LoadFile(input_name);

    string greeting = input["greeting"].as<string>();
    cout << greeting << endl;

    return 0;
}
