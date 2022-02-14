import React from 'react';
import ReactDOM from 'react-dom';

import 'fomantic-ui-css/semantic.min.css';

import { App } from './views/App';
import { configureStore } from './state/store';
import { Provider } from 'react-redux';

const store = configureStore();

ReactDOM.render(
    <Provider store={store}>
        <App />
    </Provider>,
    document.getElementById('root')
);
