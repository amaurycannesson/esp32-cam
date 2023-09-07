import { Stack } from 'aws-cdk-lib';
import { CfnApp, CfnBranch } from 'aws-cdk-lib/aws-amplify';
import { BuildSpec } from 'aws-cdk-lib/aws-codebuild';
import { Construct } from 'constructs';

interface FrontendProps {
  repository: string;
  accessToken: string;
  pubSubEndpoint: string;
  bucketName: string;
  identityPoolId: string;
  userPoolId: string;
  userPoolClientId: string;
  iotPolicyRef: string;
}

export default class Frontend extends Construct {
  constructor(scope: Construct, id: string, props: FrontendProps) {
    super(scope, id);

    const app = new CfnApp(this, 'App', {
      name: 'esp32-cam-app',
      repository: props.repository,
      accessToken: props.accessToken,
      buildSpec: BuildSpec.fromObjectToYaml({
        version: '1',
        env: {
          variables: {
            VITE_REGION: Stack.of(this).region,
            VITE_PUBSUB_ENDPOINT: props.pubSubEndpoint,
            VITE_BUCKET_NAME: props.bucketName,
            VITE_IDENTITY_POOL_ID: props.identityPoolId,
            VITE_USER_POOL_ID: props.userPoolId,
            VITE_USER_POOL_WEB_CLIENT_ID: props.userPoolClientId,
            VITE_IOT_POLICY_REF: props.iotPolicyRef,
          },
        },
        frontend: {
          phases: {
            build: {
              commands: ['cd ./frontend', 'npm i', 'npm run build'],
            },
          },
          artifacts: {
            baseDirectory: './frontend/dist',
            files: ['**/*'],
          },
          cache: {
            paths: ['node_modules/**/*'],
          },
        },
      }).toBuildSpec(),
    });

    new CfnBranch(this, 'Branch', {
      appId: app.attrAppId,
      branchName: 'main',
    });
  }
}
