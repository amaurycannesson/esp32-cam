#!/usr/bin/env node
import 'source-map-support/register';
import * as cdk from 'aws-cdk-lib';
import { Esp32CamStack } from '../lib/Esp32CamStack';

const app = new cdk.App();

new Esp32CamStack(app, 'Esp32CamStack', {
  env: {
    region: 'eu-west-3',
  },
});
