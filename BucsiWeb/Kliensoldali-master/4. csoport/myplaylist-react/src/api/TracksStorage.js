import Nedb from 'nedb/browser-version/out/nedb';
import { NedbStorage } from './NedbStorage';

const tracksDB = new Nedb({ filename: 'tracks.nedb', autoload: true });
export const tracksStorage = new NedbStorage(tracksDB);
