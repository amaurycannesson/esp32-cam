import { create } from 'zustand';
import { CONNECTION_STATE_CHANGE, ConnectionState, PubSub } from '@aws-amplify/pubsub';
import { Hub, Storage } from 'aws-amplify';
import { HubCapsule } from '@aws-amplify/core';
import {
  AlarmState,
  PirState,
  LightState,
  PirStateMessage,
  AlarmStateMessage,
  LightStateMessage,
  PhotoMessage,
  TemperatureValueMessage,
} from './types';
import { CMD_MESSAGES, TOPICS } from './constants';

type State = {
  isInitialized: boolean;
  connectionState: ConnectionState;
  alarmState: AlarmState;
  pirState: PirState;
  lightState: LightState;
  photoUrl: string;
  temperature: string;
};

type Action = {
  init: () => void;
  takePhoto: () => void;
  triggerAlarm: () => void;
  setAlarmState: (desiredState: AlarmState) => void;
  setLightState: (desiredState: LightState) => void;
};

const useStore = create<State & Action>((set, get) => ({
  isInitialized: false,
  connectionState: ConnectionState.Disconnected,
  alarmState: AlarmState.Disabled,
  pirState: PirState.Low,
  lightState: LightState.Off,
  photoUrl: '',
  temperature: '',
  init: () => {
    if (get().isInitialized) return;

    Hub.listen('pubsub', (data: HubCapsule) => {
      const { payload } = data;
      if (payload.event === CONNECTION_STATE_CHANGE) {
        const connectionState = payload.data.connectionState as ConnectionState;
        set(() => ({ connectionState }));
      }
    });

    PubSub.subscribe<PhotoMessage>(TOPICS.CAM_PHOTO).subscribe({
      next: async ({ value }) => {
        const photoUrl = await Storage.get(value.photo);
        set(() => ({ photoUrl }));
      },
      error: console.error,
    });

    PubSub.subscribe<PirStateMessage>(TOPICS.PIR_STATE).subscribe({
      next: ({ value }) => set(() => ({ pirState: value.state })),
      error: console.error,
    });

    PubSub.subscribe<AlarmStateMessage>(TOPICS.ALARM_STATE).subscribe({
      next: ({ value }) => set(() => ({ alarmState: value.state })),
      error: console.error,
    });

    PubSub.subscribe<LightStateMessage>(TOPICS.LIGHT_STATE).subscribe({
      next: ({ value }) => set(() => ({ lightState: value.state })),
      error: console.error,
    });

    PubSub.subscribe<TemperatureValueMessage>(TOPICS.TEMPERATURE_VALUE).subscribe({
      next: ({ value }) => set(() => ({ temperature: value.value.toFixed(2) })),
      error: console.error,
    });

    set(() => ({ isInitialized: true }));
  },
  takePhoto: () => {
    PubSub.publish(TOPICS.CAM_CMD, CMD_MESSAGES.CAM_PHOTO);
  },
  triggerAlarm: () => {
    PubSub.publish(TOPICS.ALARM_CMD, CMD_MESSAGES.ALARM_BEEP);
  },
  setAlarmState: (desiredState: AlarmState) => {
    PubSub.publish(
      TOPICS.ALARM_CMD,
      desiredState === AlarmState.Enabled ? CMD_MESSAGES.ALARM_ENABLE : CMD_MESSAGES.ALARM_DISABLE
    );
  },
  setLightState: (desiredState: LightState) => {
    PubSub.publish(
      TOPICS.LIGHT_CMD,
      desiredState === LightState.On ? CMD_MESSAGES.LIGHT_ON : CMD_MESSAGES.LIGHT_OFF
    );
  },
}));

export { useStore };
