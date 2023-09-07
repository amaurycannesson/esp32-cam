declare module '@aws-amplify/pubsub' {
  import Observable from 'zen-observable-ts';
  import { PubSubProvider, ProviderOptions } from '@aws-amplify/pubsub/lib//types';
  import { InternalPubSubClass } from '@aws-amplify/pubsub/lib/internals';
  declare type PubSubObservable<T> = {
    provider: PubSubProvider;
    value: T;
  };
  export declare class PubSubClass extends InternalPubSubClass {
    getModuleName(): string;
    subscribe<T>(
      topics: string[] | string,
      options?: ProviderOptions
    ): Observable<PubSubObservable<T>>;
  }
  export declare const PubSub: PubSubClass;

  export { CONNECTION_STATE_CHANGE } from '@aws-amplify/pubsub/lib/Providers';
  export { ConnectionState, CONTROL_MSG } from '@aws-amplify/pubsub/lib/types';
  export {
    AWSAppSyncRealTimeProvider,
    AWSAppSyncRealTimeProviderOptions,
    AWSIoTProvider,
    AWSIoTProviderOptions,
    AbstractPubSubProvider,
    MqttOverWSProvider,
    MqttProviderOptions,
    ObserverQuery,
    mqttTopicMatch,
  } from '@aws-amplify/pubsub/lib/Providers';
}
