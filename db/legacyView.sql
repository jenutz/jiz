CREATE OR REPLACE VIEW byhour AS
SELECT
    date,hour,
    string_agg(CONCAT(status, '-', length), ' ') AS concatenated_status
FROM extended
GROUP BY date, hour
ORDER BY date, hour;
