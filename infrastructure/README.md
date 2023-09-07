# Infrastructure

Deploy AWS resources using CDK.

## Setup

### Generate IoT Certificate

```bash
openssl genrsa -out ./cert/private-key.pem.key 2048
openssl req -new -subj "/C=FR/ST=Paris/L=Paris/O=Home/CN=AWS IoT Certificate" -key ./cert/private-key.pem.key -out ./cert/certificate-request.csr
```

### Add sandbox phone number

```bash
aws sns create-sms-sandbox-phone-number --phone-number <phone>
aws sns verify-sms-sandbox-phone-number --phone-number <phone> --one-time-password <OTP>
```

### Environment variables

* `REPOSITORY` : GitHub repository to pull frontend from.
* `GITHUB_ACCESS_TOKEN` : Give Amplify access to the repo
* `API_KEY` : Api Gateway key (20 char. min.)
* `PUBSUB_ENDPOINT` : IoT Core MQTT endpoint (need to be retried from AWS IoT). 
* `PHONE_NUMBERS` : Phone numbers (comma-separated) to receive notifications

## Run

```
 cdk deploy --verbose --outputs-file ./cdk-outputs.json
```