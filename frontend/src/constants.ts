export const TOPICS = {
  CAM_PHOTO: 'esp32/cam/photo',
  CAM_CMD: 'esp32/cam/command',
  ALARM_STATE: 'esp32/alarm/state',
  ALARM_CMD: 'esp32/alarm/command',
  LIGHT_STATE: 'esp32/light/state',
  LIGHT_CMD: 'esp32/light/command',
  PIR_STATE: 'esp32/pir/state',
  TEMPERATURE_VALUE: 'esp32/temperature/value',
};

export const CMD_MESSAGES = {
  CAM_PHOTO: { cmd: 'photo' },
  ALARM_BEEP: { cmd: 'beep' },
  ALARM_ENABLE: { cmd: 'enable' },
  ALARM_DISABLE: { cmd: 'disable' },
  LIGHT_ON: { cmd: 'on' },
  LIGHT_OFF: { cmd: 'off' },
};
