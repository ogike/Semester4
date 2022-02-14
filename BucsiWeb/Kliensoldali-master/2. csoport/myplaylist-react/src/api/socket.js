import io from "socket.io-client";

let socket = null;

class SocketApi {
    connect() {
        socket = io('http://localhost:3030');
    }

    sendMessage(message) {
        socket.emit('create', 'messages', {
            id: Date.now().toString(),
            ...message,
        });
    }

    onReceiveMessage(callback) {
        const listener = message => {
            callback(message);
        }
        socket.on('messages created', listener);
    }
}

export const socketApi = new SocketApi();
