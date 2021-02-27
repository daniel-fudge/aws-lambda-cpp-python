//
// Created by Daniel on 2021-02-24.
//

#include "s3.h"

bool S3::ReadYaml(const std::string& object_name, const std::string& bucket_name, const std::string& region_name, YAML::Node& node)
{
    
    const Aws::String fromBucket = bucket_name;
    const Aws::String objectKey = object_name;
    const Aws::String region = region_name;
    
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    Aws::Client::ClientConfiguration config;

    if (!region.empty())
    {
        config.region = region;
    }

    Aws::S3::S3Client s3_client(config);

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.SetBucket(fromBucket);
    object_request.SetKey(objectKey);

    Aws::S3::Model::GetObjectOutcome get_object_outcome = s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess())
    {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().GetBody();

        node = YAML::Load(retrieved_file);
        
        return true;
    }
    else
    {
        auto err = get_object_outcome.GetError();
        std::cout << "Error: GetObject: " << err.GetExceptionName() << ": " << err.GetMessage() << std::endl;

        return false;
    }
    Aws::ShutdownAPI(options);    

}
