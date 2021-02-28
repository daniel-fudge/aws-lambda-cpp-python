from subprocess import run

def lambda_handler(event, context):
    
    print("Event:")
    print(event)
    
    cmd = ["./cpp.exe", 
           event['Records'][0]['s3']['bucket']['name'], 
           event['Records'][0]['s3']['object']['key'], 
           event['Records'][0]['awsRegion']]
    
    print("Command:  " + " ".join(cmd))
    run(cmd)
    
    return 0