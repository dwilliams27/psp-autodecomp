// gcDesiredEntityHelper::GetRelationshipText(gcDesiredEntityHelper::gcRelationship) static
// Address: 0x0011d3a0, Size: 76B
// Obj: gcAll_psp.obj

class gcDesiredEntityHelper {
public:
    enum gcRelationship {};
    static const char* GetRelationshipText(gcRelationship r);
};

const char* gcDesiredEntityHelper::GetRelationshipText(gcRelationship r)
{
    switch (r) {
    case 0:
        return "Self";
    case 1:
        return "Friend";
    case 2:
        return "Enemy";
    default:
        return "Neutral";
    }
}
