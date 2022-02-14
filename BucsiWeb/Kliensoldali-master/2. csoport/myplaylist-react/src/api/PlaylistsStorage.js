import Nedb from 'nedb/browser-version/out/nedb';

const promisify = (fn) => (...args) => {
    return new Promise((resolve, reject) => {
        fn(...args, (err, data) => {
            if (err) {
                reject(err);
            } else {
                resolve(data);
            }
        });
    });
};

const promisifyNedb = (nedb) => {
    const db = {};
    const methods = ['find', 'findOne', 'insert', 'update', 'remove'];
    methods.forEach(
        (method) => (db[method] = promisify(nedb[method].bind(nedb)))
    );
    return db;
};

class PlaylistsStorage {
    constructor(nedb) {
        this.db = promisifyNedb(nedb);
    }

    async create(playlist) {
        if (playlist.id) {
            playlist._id = playlist.id;
        }
        const newPlaylist = await this.db.insert(playlist);
        newPlaylist.id = newPlaylist._id;
        return newPlaylist;
    }

    async fill(playlists) {
        await this.db.remove({}, { multi: true });
        const newPlaylists = await Promise.all(
            playlists.map((playlist) => this.create(playlist))
        );
        return newPlaylists;
    }

    async getAll() {
        const playlists = await this.db.find({});
        return playlists.map((playlist) => ({ ...playlist, id: playlist._id }));
    }
}

const playlistDB = new Nedb({ filename: 'playlists.nedb', autoload: true });
export const playlistsStorage = new PlaylistsStorage(playlistDB);
