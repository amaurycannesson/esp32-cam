import { Stack, StackProps } from 'aws-cdk-lib';
import { Construct } from 'constructs';
import DevicePubSub from './DevicePubSub';
import Frontend from './Frontend';
import Auth from './Auth';
import PhotoHosting from './PhotoHosting';
import Api from './Api';
import Notification from './Notification';

export class Esp32CamStack extends Stack {
  constructor(scope: Construct, id: string, props?: StackProps) {
    super(scope, id, props);

    const auth = new Auth(this, 'Auth');

    const pubSub = new DevicePubSub(this, 'DevicePubSub', { authRole: auth.authRole });

    const api = new Api(this, 'Api', { apiKey: process.env.API_KEY! });

    const photoHosting = new PhotoHosting(this, 'PhotoHosting', {
      authRole: auth.authRole,
      api: api.api,
    });

    new Frontend(this, 'Frontend', {
      repository: process.env.REPOSITORY!,
      accessToken: process.env.GITHUB_ACCESS_TOKEN!,
      pubSubEndpoint: process.env.PUBSUB_ENDPOINT!,
      bucketName: photoHosting.bucketName,
      userPoolId: auth.userPoolId,
      userPoolClientId: auth.userPoolClientId,
      identityPoolId: auth.identityPoolId,
      iotPolicyRef: pubSub.iotPolicyRef,
    });

    new Notification(this, 'Notification', { phoneNumbers: process.env.PHONE_NUMBERS!.split(',') });
  }
}
