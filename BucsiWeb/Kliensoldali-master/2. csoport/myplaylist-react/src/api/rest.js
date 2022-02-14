// const ACCESS_TOKEN = 'eyJhbGciOiJIUzI1NiIsInR5cCI6ImFjY2VzcyJ9.eyJpYXQiOjE2MTk2MTIxNDYsImV4cCI6MTYyMjIwNDE0NiwiYXVkIjoiaHR0cHM6Ly95b3VyZG9tYWluLmNvbSIsImlzcyI6ImZlYXRoZXJzIiwic3ViIjoiMSIsImp0aSI6IjkyNWVmN2UwLTNiNDctNDhkZi05ZTJhLTUxYzhjNDU2ODhjZiJ9.Wuktapqj3fgOpfe7oPOUMMyXQtJ845NuyINvOun061A';

const BASE_URL = 'http://localhost:3030';

// let token = null;

const request = async (path = '', options = {}, token, userId) => {
    let url = `${BASE_URL}${path}`;
    const headers = {
        // Authorization: `Bearer ${token}`,
        'Content-Type': 'application/json',
        Accept: 'application/json',
        ...options.headers,
    };
    if(token) {
        headers.Authorization = `Bearer ${token}`;
    }
    if(userId) {
        url += `?userId=${userId}`;
    }
    const response = await fetch(url, {
        ...options,
        headers
    });
    return response.json();
};

class RestApi {
    constructor(resourcePath, convertFn) {
        this.resourcePath = resourcePath;
        this.convertFn = convertFn;
    }

    async create(object,token) {
        const newPlaylist = await request(this.resourcePath, {
            method: 'POST',
            body: JSON.stringify(object),
        }, token);
        return this.convertFn(newPlaylist);
    }

    async fill(objects) {
        // await this.db.remove({}, { multi: true });
        // const newObjects = await Promise.all(
        //     objects.map((object) => this.create(object))
        // );
        const newObjects = objects;
        return newObjects;
    }

    async getAll(token, userId) {
        const json = await request(this.resourcePath, {}, token, userId);
        const playlists = json.data;
        return playlists.map(this.convertFn);
    }

    async update(object,token) {
        if (!object.id) return;
        const updatedPlaylist = await request(`${this.resourcePath}/${object.id}`, {
            method: 'PUT',
            body: JSON.stringify(object),
        }, token);
        return this.convertFn(updatedPlaylist);
    }

    async delete(id,token) {
        await request(`${this.resourcePath}/${id}`, {
            method: 'DELETE',
        }, token);
    }
}

const convertPlaylistIds = (playlist) => ({
    ...playlist,
    id: playlist.id.toString(),
    tracks: playlist.tracks.map((id) => id.toString()),
});

export const playlistsApi = new RestApi('/playlists', convertPlaylistIds);

const convertTrackId = (track) => ({
    ...track,
    id: track.id.toString(),
});

export const tracksApi = new RestApi('/tracks', convertTrackId);




class AuthApi {
    async login(username, password) {
        const authData = {
            email: username,
            password,
            strategy: 'local',
        };
        const response = await request('/authentication', {
            method: 'POST',
            body: JSON.stringify(authData),
        });
        window.sessionStorage.setItem('api-token', response.accessToken);
        // token = response.accessToken;
        return response;
    }

    logout() {
        window.sessionStorage.removeItem('api-token');
    }

    getToken() {
        return window.sessionStorage.getItem('api-token');
    }

    async getUserById(userId, token) {
        return await request(`/users/${userId}`, {}, token);
    }
}

export const authApi = new AuthApi();
