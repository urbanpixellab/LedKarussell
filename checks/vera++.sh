# Set up local profile vera++.profile with:
#    sudo ln -s /home/.../workspace/LedKarussell/checks/vera++.profile \
#    /usr/lib/vera++/profiles/vera++.profile
# See also https://bitbucket.org/verateam/vera/issues/60/allow-to-use-absolute-path-for-profile

cd ..

# Header file config.h is omitted.

vera++ \
-s -p vera++.profile -P max-line-length=80 src/*.[ch]pp \
-o checks/vera++.txt

#vera++ \
#-s -p vera++.profile -P max-line-length=80 src/*.[ch]pp \
#-x checks/vera++.xml

cd checks
