import { Effect, PolicyStatement, Role, ServicePrincipal } from 'aws-cdk-lib/aws-iam';
import { CfnTopicRule } from 'aws-cdk-lib/aws-iot';
import { Subscription, SubscriptionProtocol, Topic } from 'aws-cdk-lib/aws-sns';
import { Construct } from 'constructs';

interface NotificationProps {
  phoneNumbers: string[];
}

export default class Notification extends Construct {
  constructor(scope: Construct, id: string, props: NotificationProps) {
    super(scope, id);

    const topic = new Topic(this, 'Topic');

    props.phoneNumbers.forEach((phoneNumber, index) => {
      new Subscription(this, `Subscrition${index}`, {
        topic: topic,
        protocol: SubscriptionProtocol.SMS,
        endpoint: phoneNumber,
      });
    });

    const role = new Role(this, 'IoTSNSRole', {
      assumedBy: new ServicePrincipal('iot.amazonaws.com'),
    });

    role.addToPolicy(
      new PolicyStatement({
        effect: Effect.ALLOW,
        resources: [topic.topicArn],
        actions: ['sns:Publish'],
      })
    );

    new CfnTopicRule(this, 'TopicRule', {
      topicRulePayload: {
        sql: `select * from 'esp32/notification'`,
        awsIotSqlVersion: '2016-03-23',
        actions: [
          {
            sns: {
              roleArn: role.roleArn,
              targetArn: topic.topicArn,
              messageFormat: 'RAW',
            },
          },
        ],
      },
    });
  }
}
