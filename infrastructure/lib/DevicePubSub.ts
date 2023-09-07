import { CfnOutput, Stack } from 'aws-cdk-lib';
import { Effect, PolicyDocument, PolicyStatement, Role } from 'aws-cdk-lib/aws-iam';
import {
  CfnCertificate,
  CfnPolicy,
  CfnPolicyPrincipalAttachment,
  CfnThing,
  CfnThingPrincipalAttachment,
} from 'aws-cdk-lib/aws-iot';
import { Construct } from 'constructs';
import * as fs from 'fs';
import * as path from 'path';

interface DevicePubSubProps {
  authRole: Role;
}

export default class DevicePubSub extends Construct {
  readonly iotPolicyRef: string;

  constructor(scope: Construct, id: string, props: DevicePubSubProps) {
    super(scope, id);

    const thing = new CfnThing(this, 'Thing', {
      thingName: 'esp32-cam',
    });

    const cert = new CfnCertificate(this, 'Certificate', {
      status: 'ACTIVE',
      certificateSigningRequest: fs.readFileSync(
        path.resolve('cert/certificate-request.csr'),
        'utf-8'
      ),
    });

    new CfnThingPrincipalAttachment(this, 'ThingCertAttachment', {
      thingName: thing.thingName || 'esp32-cam',
      principal: cert.attrArn,
    });

    const accessIotPolicyStmt = new PolicyStatement({
      effect: Effect.ALLOW,
      actions: ['iot:*'],
      resources: [`arn:aws:iot:${Stack.of(this).region}:${Stack.of(this).account}:*`],
    });

    const iotPolicy = new CfnPolicy(this, 'Policy', {
      policyDocument: new PolicyDocument({ statements: [accessIotPolicyStmt] }),
    });

    new CfnPolicyPrincipalAttachment(this, 'CertPolicyAttachment', {
      policyName: iotPolicy.ref || 'esp32-cam-policy',
      principal: cert.attrArn,
    });

    props.authRole.addManagedPolicy({
      managedPolicyArn: 'arn:aws:iam::aws:policy/AWSIoTDataAccess',
    });

    props.authRole.addToPolicy(
      new PolicyStatement({
        effect: Effect.ALLOW,
        actions: ['iot:AttachPolicy'],
        resources: ['*'],
      })
    );

    new CfnOutput(this, 'IotThingName', { value: thing.thingName || '' });

    this.iotPolicyRef = iotPolicy.ref;
    new CfnOutput(this, 'IoTPolicy', { value: iotPolicy.ref });
  }
}
