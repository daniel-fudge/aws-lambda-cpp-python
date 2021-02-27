//
// Created by Daniel on 2021-02-24.
//

#ifndef AWS_LAMBDA_CPP_PYTHON_S3_H
#define AWS_LAMBDA_CPP_PYTHON_S3_H

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/BucketLocationConstraint.h>
#include <aws/s3/model/GetObjectRequest.h>

#include "yaml-cpp/yaml.h"

namespace S3 {
  bool ReadYaml(const std::string& object_name, const std::string& bucket_name, const std::string& region_name, YAML::Node& node);
}


#endif //AWS_LAMBDA_CPP_PYTHON_S3_H
