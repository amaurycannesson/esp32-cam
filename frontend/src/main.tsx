import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './App.tsx';
import '@aws-amplify/ui-react/styles.css';
import '@fontsource/inter';
import '@fontsource/inter/800.css';
import './awsConfigure.ts';
import './index.css';

ReactDOM.createRoot(document.getElementById('root')!).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);
