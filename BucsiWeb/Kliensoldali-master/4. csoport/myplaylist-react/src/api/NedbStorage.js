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

export class NedbStorage {
    constructor(nedb) {
        this.db = promisifyNedb(nedb);
    }

    async create(object) {
        if (object.id) {
            object._id = object.id;
        }
        const newObject = await this.db.insert(object);
        newObject.id = newObject._id;
        return newObject;
    }

    async fill(objects) {
        await this.db.remove({}, { multi: true });
        const newObjects = await Promise.all(
            objects.map((object) => this.create(object))
        );
        return newObjects;
    }

    async getAll() {
        const objects = await this.db.find({});
        return objects.map(object => ({...object, id: object._id}));
    }

    async update(object) {
        if (!object.id) return;
        object._id = object.id;
        await this.db.update({ _id: object.id }, object, {});
        return object;
    }

    async delete(id) {
        await this.db.remove({ _id: id });
    }
}