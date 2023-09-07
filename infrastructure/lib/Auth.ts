import { CfnOutput } from 'aws-cdk-lib';
import {
  CfnIdentityPool,
  CfnIdentityPoolRoleAttachment,
  CfnUserPool,
  CfnUserPoolClient,
} from 'aws-cdk-lib/aws-cognito';
import { Role, WebIdentityPrincipal } from 'aws-cdk-lib/aws-iam';
import { Construct } from 'constructs';

export default class Auth extends Construct {
  readonly authRole: Role;
  readonly identityPoolId: string;
  readonly userPoolId: string;
  readonly userPoolClientId: string;

  constructor(scope: Construct, id: string) {
    super(scope, id);

    const userPool = new CfnUserPool(this, 'UserPool', {
      adminCreateUserConfig: { allowAdminCreateUserOnly: true },
    });

    const userPoolClient = new CfnUserPoolClient(this, 'UserPoolClient', {
      userPoolId: userPool.ref,
    });

    const identityPool = new CfnIdentityPool(this, 'IdentityPool', {
      allowUnauthenticatedIdentities: false,
      cognitoIdentityProviders: [
        {
          clientId: userPoolClient.ref,
          providerName: userPool.attrProviderName,
          serverSideTokenCheck: true,
        },
      ],
    });

    this.authRole = new Role(this, 'AuthRole', {
      assumedBy: new WebIdentityPrincipal('cognito-identity.amazonaws.com', {
        StringEquals: {
          'cognito-identity.amazonaws.com:aud': identityPool.ref,
        },
        'ForAnyValue:StringLike': {
          'cognito-identity.amazonaws.com:amr': 'authenticated',
        },
      }),
    });

    new CfnIdentityPoolRoleAttachment(this, 'IdentityPoolRoleAttachment', {
      identityPoolId: identityPool.ref,
      roles: { authenticated: this.authRole.roleArn },
    });

    this.identityPoolId = identityPool.ref;
    new CfnOutput(this, 'identityPoolId', {
      value: identityPool.ref,
    });

    this.userPoolId = userPool.ref;
    new CfnOutput(this, 'userPoolId', {
      value: userPool.ref,
    });

    this.userPoolClientId = userPoolClient.ref;
    new CfnOutput(this, 'userPoolClientId', {
      value: userPoolClient.ref,
    });
  }
}
