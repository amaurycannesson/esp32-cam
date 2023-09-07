import * as path from 'path';
import { Construct } from 'constructs';
import { Effect, PolicyStatement, Role } from 'aws-cdk-lib/aws-iam';
import { Bucket, HttpMethods } from 'aws-cdk-lib/aws-s3';
import { NodejsFunction } from 'aws-cdk-lib/aws-lambda-nodejs';
import { LambdaIntegration, RestApi } from 'aws-cdk-lib/aws-apigateway';
import { Runtime } from 'aws-cdk-lib/aws-lambda';
import { CfnOutput, Duration, RemovalPolicy } from 'aws-cdk-lib';

interface PhotoHostingProps {
  authRole: Role;
  api: RestApi;
}

export default class PhotoHosting extends Construct {
  readonly bucketName: string;

  constructor(scope: Construct, id: string, props: PhotoHostingProps) {
    super(scope, id);

    const bucket = new Bucket(this, 'Bucket', {
      removalPolicy: RemovalPolicy.DESTROY,
      autoDeleteObjects: true,
      lifecycleRules: [
        {
          expiration: Duration.days(15),
        },
      ],
      cors: [
        {
          allowedHeaders: ['*'],
          allowedMethods: [HttpMethods.GET],
          allowedOrigins: ['*'],
          exposedHeaders: [
            'x-amz-server-side-encryption',
            'x-amz-request-id',
            'x-amz-id-2',
            'ETag',
          ],
          maxAge: 3000,
        },
      ],
    });

    const nodeJsFunction = new NodejsFunction(this, 'NodejsFunction', {
      runtime: Runtime.NODEJS_18_X,
      entry: path.join(__dirname, '../../functions/save-photo-to-s3/src/index.ts'),
      environment: {
        BUCKET: bucket.bucketName,
      },
      bundling: {
        externalModules: ['@aws-sdk/client-s3'],
      },
    });

    bucket.grantPut(nodeJsFunction);

    const lambdaIntegration = new LambdaIntegration(nodeJsFunction, {
      allowTestInvoke: false,
    });

    props.api.root
      .addResource('save-photo-to-s3')
      .addMethod('POST', lambdaIntegration, { apiKeyRequired: true });

    props.authRole.addToPolicy(
      new PolicyStatement({
        effect: Effect.ALLOW,
        actions: ['s3:GetObject'],
        resources: [`arn:aws:s3:::${bucket.bucketName}/*`],
      })
    );

    this.bucketName = bucket.bucketName;
    new CfnOutput(this, 'bucketName', { value: bucket.bucketName });
  }
}
