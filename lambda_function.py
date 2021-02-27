from os import environ
from subprocess import run

def lambda_handler(event, context):
    
    cmd = ["./cpp.exe", event["bucket"], event["object"], environ["AWS_REGION"]]
    print(" ".join(cmd))
    run(cmd)
    
    return 0
    