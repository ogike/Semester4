import Nedb from 'nedb/browser-version/out/nedb';
import { NedbStorage } from './NedbStorage';

const playlistDB = new Nedb({ filename: 'playlists.nedb', autoload: true });
export const playlistsStorage = new NedbStorage(playlistDB);
