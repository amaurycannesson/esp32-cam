import { Construct } from 'constructs';
import { Period, RestApi } from 'aws-cdk-lib/aws-apigateway';

interface ApiProps {
  apiKey: string;
}

export default class Api extends Construct {
  readonly api: RestApi;

  constructor(scope: Construct, id: string, props: ApiProps) {
    super(scope, id);

    this.api = new RestApi(this, 'RestApi');
    const apiKey = this.api.addApiKey('ApiKey', { value: props.apiKey });
    const usagePlan = this.api.addUsagePlan('UsagePlan', {
      throttle: {
        rateLimit: 1,
        burstLimit: 1,
      },
      quota: { limit: 100, period: Period.DAY },
    });

    usagePlan.addApiStage({ stage: this.api.deploymentStage });
    usagePlan.addApiKey(apiKey);
  }
}
