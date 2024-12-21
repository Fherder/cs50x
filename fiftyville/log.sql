-- Keep a log of any SQL queries you execute as you solve the mystery.

SELECT id, description
FROM crime_scene_reports
WHERE day = 28 AND month = 7 AND year = 2023 AND street = 'Humphrey Street';

SELECT transcript
FROM interviews
WHERE day = 28 AND month = 7 AND year = 2023 AND transcript LIKE '%bakery%';

-- Car owners:
SELECT name, b.license_plate, hour, minute, activity
FROM people
JOIN bakery_security_logs b ON people.license_plate = b.license_plate
WHERE b.day = 28 AND b.month = 7 AND b.year = 2023 AND b.hour = 10 AND b.minute > 15 AND b.minute < 25;

-- Bank accounts:
SELECT name,  b.account_number
FROM people
JOIN bank_accounts b ON people.id = b.person_id
WHERE b.account_number IN (
    SELECT account_number
    FROM atm_transactions
    WHERE day = 28 AND month = 7 AND year = 2023 AND atm_location =  'Leggett Street' AND transaction_type = 'withdraw'
);

-- Caller and Receiver names combined:
SELECT c.name AS caller_name, c.phone_number AS caller_phone, r.name AS receiver_name, r.phone_number AS receiver_phone
FROM phone_calls p
JOIN people c ON p.caller = c.phone_number -- Caller
JOIN people r ON p.receiver = r.phone_number -- Receiver
WHERE p.day = 28 AND p.month = 7 AND p.year = 2023
AND p.duration < 60;

-- Intersect (Names that appear in all the tables)
SELECT name
FROM people
JOIN bakery_security_logs b ON people.license_plate = b.license_plate
WHERE b.day = 28 AND b.month = 7 AND b.year = 2023 AND b.hour = 10 AND b.minute > 15 AND b.minute < 25

INTERSECT

SELECT name
FROM people
JOIN bank_accounts b ON people.id = b.person_id
WHERE b.account_number IN (
    SELECT account_number
    FROM atm_transactions
    WHERE day = 28 AND month = 7 AND year = 2023 AND atm_location =  'Leggett Street' AND transaction_type = 'withdraw'
)

INTERSECT

SELECT name
FROM people
JOIN phone_calls p ON people.phone_number = p.caller
WHERE day = 28 AND month = 7 AND duration < 60;

-- Diana and Bruce are the only two THIEF suspects

-- The earliest flight out of Fiftyville
SELECT *
FROM flights
WHERE day = 29 AND month = 7 AND year = 2023
ORDER BY hour, minute;

-- The earliest flight is the flight 36

-- Diana or Bruce?
SELECT p.name
FROM passengers
JOIN flights f ON passengers.flight_id = f.id
JOIN people p ON passengers.passport_number = p.passport_number
WHERE passengers.flight_id = 36 AND f.day = 29 AND f.year = 2023 AND f.month = 7 AND f.hour = 8;

-- Bruce is the thief

-- Where is Bruce heading to?
SELECT city, id
FROM airports
WHERE id = (
    SELECT destination_airport_id
    FROM flights
    WHERE id = (
        SELECT flight_id
        FROM passengers
        WHERE passport_number = (
            SELECT passport_number
            FROM people
            WHERE name = 'Bruce'
        )
    )
    AND day = 29 AND month = 7 AND YEAR = 2023
);

