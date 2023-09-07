import { useAuthenticator } from '@aws-amplify/ui-react';
import { useEffect } from 'react';
import { Auth } from 'aws-amplify';
import { AttachPolicyCommand, IoTClient } from '@aws-sdk/client-iot';

const useAutoAttachIotPolicy = () => {
  const { authStatus } = useAuthenticator((context) => [context.authStatus]);

  useEffect(() => {
    if (authStatus == 'authenticated') {
      Auth.currentCredentials().then(async (credentials) => {
        const iotClient = new IoTClient({
          region: import.meta.env.VITE_REGION,
          credentials: Auth.essentialCredentials(credentials),
        });
        const cmdParams = {
          policyName: import.meta.env.VITE_IOT_POLICY_REF,
          target: credentials.identityId,
        };

        try {
          await iotClient.send(new AttachPolicyCommand(cmdParams));
        } catch (error) {
          console.error(error);
        }
      });
    }
  }, [authStatus]);
};

export { useAutoAttachIotPolicy };
