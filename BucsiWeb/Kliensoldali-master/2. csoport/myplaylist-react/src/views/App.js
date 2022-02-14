import { Layout } from './layout/Layout';
import { Home } from './home/Home';
import { Playlists } from './playlists/Playlists';
import { Tracks } from './tracks/Tracks';
import { Search } from './search/Search';
import { BrowserRouter, Redirect, Route, Switch } from 'react-router-dom';
import { useDispatch, useSelector } from 'react-redux';
import { useEffect } from 'react';
import { fetchPlaylists } from '../state/playlists/actions';
import { fetchTracks } from '../state/tracks/actions';
import { wsConnect } from '../state/messages/actions';
import { getIsLoggedIn } from '../state/auth/selectors';
import { restoreUser } from '../state/auth/actions';

export function App() {
    const dispatch = useDispatch();
    const isLoggedIn = useSelector(getIsLoggedIn);

    useEffect(() => {
        // playlistsStorage.fill(examplePlaylists).then(playlists => dispatch(setPlaylists(playlists)));
        // tracksStorage.fill(exampleTracks).then(tracks => dispatch(setTracks(tracks)));

        if(isLoggedIn) {
            dispatch(fetchPlaylists());
            dispatch(fetchTracks());
        }
    }, [dispatch, isLoggedIn]);

    useEffect(() => {
        dispatch(wsConnect());
    }, [dispatch]);

    useEffect(() => {
        dispatch(restoreUser())
    }, [dispatch])

    return (
        <BrowserRouter>
            <Layout>
                <Switch>
                    <Route exact path="/">
                        <Home />
                    </Route>
                    <Route path="/playlists/:playlistId?/:trackId?">
                        {isLoggedIn ? <Playlists /> : <Redirect to="/" />}
                    </Route>
                    {isLoggedIn && (
                        <Route path="/tracks">
                            <Tracks />
                        </Route>
                    )}
                    <PrivateRoute path="/search">
                        <Search />
                    </PrivateRoute>
                    <Redirect to="/" />
                </Switch>
            </Layout>
        </BrowserRouter>
    );
}

const PrivateRoute = ({ children, ...props }) => {
    const isLoggedIn = useSelector(getIsLoggedIn);
    return (
        <Route {...props}>
            {isLoggedIn ? children : <Redirect to="/" />}
        </Route>
    );
};
