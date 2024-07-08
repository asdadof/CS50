SELECT DISTINCT p.name
FROM people AS p
JOIN stars AS s ON p.id = s.person_id
JOIN movies AS m ON s.movie_id = m.id
JOIN stars AS s_kb ON m.id = s_kb.movie_id
JOIN people AS kb ON s_kb.person_id = kb.id
WHERE kb.name = 'Kevin Bacon' AND kb.birth = 1958
AND p.name != 'Kevin Bacon';
