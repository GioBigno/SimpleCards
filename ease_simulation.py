import math

MIN_EASE = 1.3

def get_mastery_level(ease, repetitions):
    if ease < 2.05 and repetitions < 2:
        return "New"
    elif ease < 2.1 or repetitions < 6:
        return "Learning"
    else:
        return "Master"

def update_card_state(result, ease, repetitions, interval):
    if result == -1:
        repetitions = 0
        interval = 1
        ease = max(MIN_EASE, ease - 0.1)
    elif result == 0:
        repetitions += 1
        interval = round(interval * 1.2)
        ease = max(MIN_EASE, ease - 0.05)
    else:
        repetitions += 1
        interval = round(interval * ease)
        ease += 0.03
    return ease, repetitions, interval

def interactive_simulation():
    ease = 2.0
    repetitions = 0
    interval = 1
    
    print("Spaced Repetition Flashcard Simulator")
    print("Enter your review result each time:")
    print("-1 = fail, 0 = okay, 1 = good")
    print("Type 'exit' to quit.\n")
    
    while True:
        mastery = get_mastery_level(ease, repetitions)
        print(f"Current state: Ease={ease:.2f}, Repetitions={repetitions}, Interval={interval} days, Mastery={mastery}")
        
        user_input = input("Enter result (-1, 0, 1): ").strip()
        if user_input.lower() == "exit":
            print("Exiting simulation.")
            break
        
        if user_input not in ['-1', '0', '1']:
            print("Invalid input, please enter -1, 0, or 1.")
            continue
        
        result = int(user_input)
        ease, repetitions, interval = update_card_state(result, ease, repetitions, interval)
        print() 

if __name__ == "__main__":
    interactive_simulation()
