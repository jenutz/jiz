CREATE DATABASE jiz;
CREATE EXTENSION IF NOT EXISTS tablefunc;    

CREATE TABLE timetable(
    Id              SERIAL      PRIMARY KEY,
    Status          VARCHAR(30) NOT NULL,
    Date            DATE        NOT NULL,
    Time_Start      TIME        NOT NULL,
    Time_End        TIME        NOT NULL
);


CREATE OR REPLACE VIEW extended AS
SELECT 
    *, 
    (EXTRACT(EPOCH FROM (time_end - time_start)) / 60)::int AS length,
    EXTRACT(hour FROM time_start) as hour
FROM timetable;


SELECT * FROM crosstab(
    'SELECT 
        date,
        hour, 
        string_agg(CONCAT(status,'-',length), ' ') AS concatenated_status 
     FROM extended group by date,hour'
) AS extended(row_name text, H1 text, H2 text, H3 text);

SELECT * FROM crosstab(
    'SELECT 
        date,
        hour, 
        string_agg(CONCAT(status, ''-'', length), '' '') AS concatenated_status 
     FROM extended 
     GROUP BY date, hour 
     ORDER BY date, hour',
    'SELECT DISTINCT hour FROM extended ORDER BY hour'
) AS extended(
    row_name date, 
    H1 text, 
    H2 text, 
    H3 text,
    H4 text,
    H5 text,
    H6 text,
);

SELECT * FROM crosstab(
    'SELECT
        date,hour,
        string_agg(CONCAT(status, ''-'', length), '' '') AS concatenated_status
     FROM extended
     GROUP BY date, hour
     ORDER BY date, hour',
    'SELECT DISTINCT hour FROM extended ORDER BY hour'
) AS extended(
    row_name date,
    H00 text, H01 text, H02 text, H03 text, H04 text, H05 text, 
    H06 text, H07 text, H08 text, H09 text, H10 text, H11 text,
    H12 text, H13 text, H14 text, H15 text, H16 text, H17 text,
    H18 text, H19 text, H20 text, H21 text, H22 text, H23 text
);
