SELECT name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number JOIN passengers ON people.passport_number = passengers.passport_number JOIN flights ON passengers.flight_id = flights.id
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND activity = 'entrance')
AND phone_number IN (SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND duration < 60)
AND bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND transaction_type = 'withdraw')
AND origin_airport_id = 8
AND flights.day=29 GROUP BY hour LIMIT 1;
------
SELECT city FROM airports JOIN flights ON airports.id=flights.destination_airport_id JOIN passengers ON flights.id=passengers.flight_id JOIN people ON passengers.passport_number=people.passport_number
WHERE name IN (SELECT name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number JOIN passengers ON people.passport_number = passengers.passport_number JOIN flights ON passengers.flight_id = flights.id
WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE day = 28 AND month = 7 AND activity = 'entrance')
AND phone_number IN (SELECT caller FROM phone_calls WHERE day = 28 AND month = 7 AND duration < 60)
AND bank_accounts.account_number IN (SELECT account_number FROM atm_transactions WHERE month = 7 AND day = 28 AND transaction_type = 'withdraw')
AND origin_airport_id = 8
AND flights.day=29 GROUP BY hour LIMIT 1);
------


