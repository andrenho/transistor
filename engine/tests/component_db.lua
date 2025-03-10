print("Component DB")

do
   print(" - Native components are created")
   db = ComponentDB.new()
   assert(db.items["__button"] ~= nil)
end