SELECT 
    l2.status,
    EXTRACT(day FROM 
        (l2.date::timestamp - (SELECT MIN(date) FROM timediff)::timestamp)) 
        AS days,
    l2.hour
FROM
        

        (SELECT 
             s.date, s.hour, MIN(time_start) as time_start
        FROM
                (SELECT
                    date, hour, MAX(diff) as diff
                FROM
                    timediff
                GROUP BY hour, date
                ) as t
            JOIN 
                timediff as s
            ON s.hour = t.hour and t.diff = s.diff and t.date = s.date
        GROUP BY s.hour, s.date
        ) as l1


    JOIN
        timediff as l2

    ON l1.time_start = l2.time_start and l1.date = l2.date
ORDER BY id;


