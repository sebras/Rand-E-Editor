#ifdef COMMENT
	Copyright abandoned, 1983, The Rand Corporation
#endif

int ldivr;

int
ldiv (a, b)
long a;
int b;
{
    ldivr = a % b;
    return a / b;
}
