-- write a SQL query to list the names of all people who have
-- directed a movie that received a rating of at least 9.0.
SELECT name FROM people WHERE id IN (
    SELECT person_id FROM directors, ratings  WHERE directors.movie_id = ratings.movie_id AND rating >= 9.0
);
