import io from "socket.io-client";

let socket = null;

class SocketApi {
    connect() {
        socket = io('http://localhost:3030');
    }

    sendMessage(message) {
        socket.emit('create', 'messages', {
            ...message,
            emitter: socket.id,
        })
    }

    onReceiveMessage(callback) {
        const listener = message => {
            if(socket.id !== message.emitter) {
                callback(message);
            }
        }
        socket.on('messages created', listener);
        return () => socket.off('messages created', listener);
    }
}

export const socketApi = new SocketApi();
