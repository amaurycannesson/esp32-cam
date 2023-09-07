import {
  Badge,
  Button,
  Card,
  Flex,
  Heading,
  Image,
  View,
  useTheme,
  withAuthenticator,
} from '@aws-amplify/ui-react';
import { useEffect } from 'react';
import { useStore } from './useStore';
import { AlarmState, LightState, PirState } from './types';
import { useAutoAttachIotPolicy } from './useAutoAttachIotPolicy';

const TakePicture = () => {
  const takePhoto = useStore((state) => state.takePhoto);

  return (
    <Button variation="primary" onClick={takePhoto}>
      Take picture
    </Button>
  );
};

const Enable = () => {
  const setAlarmState = useStore((state) => state.setAlarmState);

  return (
    <Button variation="primary" onClick={() => setAlarmState(AlarmState.Enabled)}>
      Enable
    </Button>
  );
};

const Disable = () => {
  const setAlarmState = useStore((state) => state.setAlarmState);

  return (
    <Button variation="primary" onClick={() => setAlarmState(AlarmState.Disabled)}>
      Disable
    </Button>
  );
};

const Trigger = () => {
  const triggerAlarm = useStore((state) => state.triggerAlarm);

  return (
    <Button variation="destructive" onClick={triggerAlarm}>
      Trigger
    </Button>
  );
};

const State = ({ status }: { status: AlarmState }) => {
  if (status === AlarmState.Enabled) {
    return (
      <Badge size="large" variation="success">
        Enabled
      </Badge>
    );
  }

  if (status === AlarmState.Disabled) {
    return (
      <Badge size="large" variation="warning">
        Disabled
      </Badge>
    );
  }

  if (status === AlarmState.Beeping) {
    return (
      <Badge size="large" variation="error">
        Beeping
      </Badge>
    );
  }
};

const Turn = ({ state }: { state: LightState }) => {
  const setLightState = useStore((state) => state.setLightState);

  return (
    <Button variation="primary" onClick={() => setLightState(state)}>
      Turn {state.toLocaleUpperCase()}
    </Button>
  );
};

const Home = () => {
  useAutoAttachIotPolicy();

  const theme = useTheme();

  const init = useStore((state) => state.init);
  const pirState = useStore((state) => state.pirState);
  const lightState = useStore((state) => state.lightState);
  const alarmState = useStore((state) => state.alarmState);
  const photoUrl = useStore((state) => state.photoUrl);
  const temperature = useStore((state) => state.temperature);

  useEffect(() => {
    init();
  });

  return (
    <Flex
      justifyContent="center"
      alignItems="center"
      backgroundColor={theme.tokens.colors.background.secondary}
    >
      <View maxWidth={theme.breakpoints.values.large} width="100%">
        <Flex
          backgroundColor="white"
          boxShadow="0px 0px 8px rgba(0,0,0,.6)"
          height={64}
          alignItems="center"
        >
          <Flex alignItems="center">
            <Image src="logo.svg" alt="logo" height={48} />
            <Heading level={5} fontWeight="800">
              Owly
            </Heading>
          </Flex>
        </Flex>
        <Card variation="elevated" margin={8}>
          <Heading level={6}>Alarm</Heading>
          <Flex gap={12} margin={8} direction="column">
            <View>
              <State status={alarmState} />
            </View>
            <Flex gap={12}>
              <Enable />
              <Disable />
              <Trigger />
            </Flex>
          </Flex>
        </Card>
        <Card variation="elevated" margin={8}>
          <Heading level={6}>Movement detector</Heading>
          <Flex margin={8}>
            {pirState === PirState.Low ? (
              <Badge size="large">Nothing</Badge>
            ) : (
              <Badge size="large" variation="error">
                Detected
              </Badge>
            )}
          </Flex>
        </Card>
        <Card variation="elevated" margin={8}>
          <Heading level={6}>Camera</Heading>
          <Flex margin={8} direction="column">
            <View>
              <TakePicture />
            </View>
            {photoUrl && <Image src={photoUrl} alt="img" />}
          </Flex>
        </Card>
        <Card variation="elevated" margin={8}>
          <Heading level={6}>Light</Heading>
          <Flex gap={12} margin={8} direction="column">
            <View>
              <Badge size="large">{lightState}</Badge>
            </View>
            <Flex gap={12}>
              <Turn state={LightState.On} />
              <Turn state={LightState.Off} />
            </Flex>
          </Flex>
        </Card>
        <Card variation="elevated" margin={8}>
          <Heading level={6}>Temperature</Heading>
          <View margin={8}>
            <Badge size="large">{temperature}°C</Badge>
          </View>
        </Card>
      </View>
    </Flex>
  );
};

export default withAuthenticator(Home, { hideSignUp: true });
