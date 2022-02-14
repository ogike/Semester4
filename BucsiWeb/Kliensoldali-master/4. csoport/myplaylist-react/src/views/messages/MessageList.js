import { useSelector } from 'react-redux';
import { Message } from 'semantic-ui-react';
import classes from './MessageList.module.css';
import { getMessages } from '../../state/messages/selectors';

export function MessageList() {
    const messages = useSelector(getMessages);
    return (
        <div className={classes.MessageList}>
            {messages.map((message) => (
                <Message info key={message.id}>{message.text}</Message>
            ))}
        </div>
    );
}
