/// <reference types="vite/client" />

interface ImportMetaEnv {
  readonly VITE_REGION: string;
  readonly VITE_PUBSUB_ENDPOINT: string;
  readonly VITE_BUCKET_NAME: string;
  readonly VITE_IDENTITY_POOL_ID: string;
  readonly VITE_USER_POOL_ID: string;
  readonly VITE_USER_POOL_WEB_CLIENT_ID: string;
  readonly VITE_IOT_POLICY_REF: string;
}

interface ImportMeta {
  readonly env: ImportMetaEnv;
}
