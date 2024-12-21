-- write a SQL query to determine the number of movies with an IMDb rating of 10.0.
SELECT COUNT(id) FROM movies, ratings WHERE id = ratings.movie_id AND ratings.rating = 10.0;
