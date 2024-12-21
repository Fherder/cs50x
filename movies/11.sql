-- write a SQL query to list the titles of the five highest rated movies (in order)
-- that Chadwick Boseman starred in, starting with the highest rated.
SELECT m.title
FROM movies m
JOIN ratings r ON m.id = r.movie_id
JOIN stars s ON m.id = s.movie_id
WHERE s.person_id = (
    SELECT id FROM people WHERE name = 'Chadwick Boseman'
)
ORDER BY r.rating DESC
LIMIT 5;
