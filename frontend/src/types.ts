export enum PirState {
  High = 'high',
  Low = 'low',
}

export enum AlarmState {
  Enabled = 'enabled',
  Disabled = 'disabled',
  Beeping = 'beeping',
}

export enum LightState {
  On = 'on',
  Off = 'off',
}

export type PirStateMessage = {
  state: PirState;
};

export type PhotoMessage = {
  photo: string;
};

export type AlarmStateMessage = {
  state: AlarmState;
};

export type LightStateMessage = {
  state: LightState;
};

export type TemperatureValueMessage = {
  value: number;
};
