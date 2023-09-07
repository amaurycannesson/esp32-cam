import { Amplify } from 'aws-amplify';
import { AWSIoTProvider } from '@aws-amplify/pubsub';

Amplify.configure({
  Storage: {
    AWSS3: {
      bucket: import.meta.env.VITE_BUCKET_NAME,
      region: import.meta.env.VITE_REGION,
    },
    customPrefix: {
      public: '',
    },
  },
  Auth: {
    identityPoolId: import.meta.env.VITE_IDENTITY_POOL_ID,
    region: import.meta.env.VITE_REGION,
    userPoolId: import.meta.env.VITE_USER_POOL_ID,
    userPoolWebClientId: import.meta.env.VITE_USER_POOL_WEB_CLIENT_ID,
  },
});

Amplify.addPluggable(
  new AWSIoTProvider({
    aws_pubsub_region: import.meta.env.VITE_REGION,
    aws_pubsub_endpoint: `wss://${import.meta.env.VITE_PUBSUB_ENDPOINT}/mqtt`,
  })
);
