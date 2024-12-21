-- write a SQL query that returns the average energy of songs that are by Drake.
SELECT AVG(energy) FROM songs WHERE songs.artist_id = (SELECT id FROM artists WHERE name = 'Drake');
